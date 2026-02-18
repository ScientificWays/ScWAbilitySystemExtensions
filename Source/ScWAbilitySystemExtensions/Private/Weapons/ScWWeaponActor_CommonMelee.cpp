// Scientific Ways

#include "Weapons/ScWWeaponActor_CommonMelee.h"

#include "Tags/ScWCoreTags.h"
#include "AbilitySystem/ScWAbilitySystemGlobals.h"

#include "Animation/ScWAnimationsFunctionLibrary.h"

#include "Character/ScWCharacter.h"

#include "Damage/ScWDamageType.h"

AScWWeaponActor_CommonMelee::AScWWeaponActor_CommonMelee(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	SwingBaseDamage = 10.0f;
	SwingBaseDamageTypeClass = UScWDamageType::StaticClass();

	SwingAIMaxRange = 128.0f;
	SwingAIMaxRange_BlackboardKeyName = TEXT("MeleeRangeMax");

	SwingVariantBaseDuration = 1.0f;

	SwingParticlesAttachmentSocketName = NAME_None;
	SwingParticlesRelativeTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, 0.0f), FVector::OneVector);

	PreSwingMontageSectionIndex = 0;
	SwingMontageSectionIndex = 1;
	EndSwingMontageSectionIndex = 2;
}

//~ Begin Initialize
void AScWWeaponActor_CommonMelee::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	
}

void AScWWeaponActor_CommonMelee::BeginPlay() // AActor
{
	SwingCounter = 0;

	BP_ResetSwingComponents();

	SwingHitIgnoredActors = { GetInstigator(), this};
	TracePatternIgnoredActors = { GetInstigator(), this };

	if (UShapeComponent* CollisionComponent = BP_GetCollisionComponent())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AScWWeaponActor_CommonMelee::OnCollisionComponentBeginOverlap);
	}
	Super::BeginPlay();
}
//~ End Initialize

//~ Begin Components
void AScWWeaponActor_CommonMelee::OnCollisionComponentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepHitResult)
{
	if (InOtherActor == GetInstigator())
	{
		return;
	}
	if (bInFromSweep)
	{
		BP_HandleSwingHit(InOtherActor, InSweepHitResult);
	}
	else
	{
		FHitResult MinimalInfoHitResult;
		UScWDamageType::MakeMinimalInfoDamageImpactHitResult(this, InOverlappedComponent, GetInstigator(), InOtherActor, InOtherComponent, MinimalInfoHitResult);
		BP_HandleSwingHit(InOtherActor, MinimalInfoHitResult);
	}
}
//~ End Components

//~ Begin Swing
float AScWWeaponActor_CommonMelee::BP_GetSwingDamage_Implementation() const
{
	return SwingBaseDamage;
}

TSubclassOf<UScWDamageType> AScWWeaponActor_CommonMelee::BP_GetSwingDamageTypeClass_Implementation() const
{
	return SwingBaseDamageTypeClass;
}

void AScWWeaponActor_CommonMelee::BP_GetSwingVariants_Implementation(TArray<FScWMeleeSwingVariantData>& OutVariants) const
{
	OutVariants = SwingVariants;
}

float AScWWeaponActor_CommonMelee::BP_PreSwing_Implementation()
{
	CurrentSwingPhase = EScWSwingPhase::PreSwing;

	++SwingCounter;
	BP_UpdateCurrentSwingVariantData();

	const auto& CurrentSwingMontageData = CurrentSwingVariantData.MontageData;

	ensureReturn(CurrentSwingMontageData.GetRelevantTimingMontage(), -1.0f);
	ensureReturn(CurrentSwingMontageData.GetRelevantTimingMontage()->GetNumSections() >= 3, -1.0f);

	ACharacter* InstigatorCharacter = GetInstigator<ACharacter>();
	ensureReturn(InstigatorCharacter, -1.0f);
	UScWAnimationsFunctionLibrary::PlayCharacterMontagesFromData(InstigatorCharacter, CurrentSwingMontageData);

	float OutPreSwingDelay = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingMontageData, PreSwingMontageSectionIndex) * SwingVariantBaseDuration;
	OnPreSwingDelegate.Broadcast(OutPreSwingDelay);
	return OutPreSwingDelay;
}

float AScWWeaponActor_CommonMelee::BP_BeginSwing_Implementation(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass)
{
	CurrentSwingPhase = EScWSwingPhase::Swing;

	LastSwingDamage = InSwingDamage;
	LastSwingDamageTypeClass = InSwingDamageTypeClass;
	LastSwingAffectedActorArray.Empty();

	SwingPaticlesComponent = BP_InitializeSwingParticles();

	if (UShapeComponent* CollisionComponent = BP_GetCollisionComponent())
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		BP_BeginTracePatterns();
	}
	float OutSwingDuration = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, SwingMontageSectionIndex) * SwingVariantBaseDuration;
	OnBeginSwingDelegate.Broadcast(OutSwingDuration);
	return OutSwingDuration;
}

float AScWWeaponActor_CommonMelee::BP_EndSwing_Implementation(const bool bInWasCancelled)
{
	BP_ResetSwingComponents();

	float OutEndSwingDelay = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, EndSwingMontageSectionIndex) * SwingVariantBaseDuration;
	OnEndSwingDelegate.Broadcast(OutEndSwingDelay, bInWasCancelled);
	return OutEndSwingDelay;
}

void AScWWeaponActor_CommonMelee::BP_ResetSwingComponents_Implementation()
{
	if (SwingPaticlesComponent)
	{
		SwingPaticlesComponent->DestroyComponent();
		SwingPaticlesComponent = nullptr;
	}
	if (UShapeComponent* CollisionComponent = BP_GetCollisionComponent())
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	CurrentSwingPhase = EScWSwingPhase::None;
}

void AScWWeaponActor_CommonMelee::BP_UpdateCurrentSwingVariantData_Implementation()
{
	TArray<FScWMeleeSwingVariantData> VariantsArray;
	BP_GetSwingVariants(VariantsArray);

	ensureReturn(!VariantsArray.IsEmpty());
	CurrentSwingVariantData = VariantsArray[SwingCounter % VariantsArray.Num()];
}

bool AScWWeaponActor_CommonMelee::BP_HandleSwingHit_Implementation(AActor* InHitActor, const FHitResult& InHitResult)
{
	ensureReturn(CurrentSwingPhase == EScWSwingPhase::Swing, false);
	ensureReturn(InHitActor, false);

	if (SwingHitIgnoredActors.Contains(InHitActor) || LastSwingAffectedActorArray.Contains(InHitActor))
	{
		return false;
	}
	else
	{
		LastSwingAffectedActorArray.Add(InHitActor);

		FVector HitDirection = (InHitResult.TraceEnd - InHitResult.TraceStart).GetSafeNormal();

		ensureReturn(GetInstigator(), false);
		UScWDamageType::ApplyPointDamage(this, InHitActor, LastSwingDamage, HitDirection, InHitResult, GetInstigator()->GetController(), LastSwingDamageTypeClass);
		return true;
	}
}
//~ End Swing

//~ Begin Swing FX
USceneComponent* AScWWeaponActor_CommonMelee::BP_GetFXAttachComponent_Implementation() const
{
	ACharacter* InstigatorCharacter = Cast<ACharacter>(GetInstigator());
	return InstigatorCharacter ? InstigatorCharacter->GetMesh() : nullptr;
}

UNiagaraComponent* AScWWeaponActor_CommonMelee::BP_InitializeSwingParticles_Implementation() const
{
	if (DefaultSwingParticles)
	{
		USceneComponent* AttachToComponent = BP_GetFXAttachComponent();
		ensureReturn(AttachToComponent, nullptr);

		FFXSystemSpawnParameters SpawnParams;
		SpawnParams.WorldContextObject = this;
		SpawnParams.SystemTemplate = DefaultSwingParticles;
		SpawnParams.Location = SwingParticlesRelativeTransform.GetLocation();
		SpawnParams.Rotation = SwingParticlesRelativeTransform.GetRotation().Rotator();
		SpawnParams.Scale = SwingParticlesRelativeTransform.GetScale3D();
		SpawnParams.AttachToComponent = AttachToComponent;
		SpawnParams.AttachPointName = SwingParticlesAttachmentSocketName;
		SpawnParams.LocationType = EAttachLocation::KeepRelativeOffset;
		SpawnParams.bAutoDestroy = true;
		SpawnParams.bAutoActivate = true;
		SpawnParams.PoolingMethod = EPSCPoolMethod::None;
		SpawnParams.bPreCullCheck = true;
		SpawnParams.bIsPlayerEffect = UScWAbilitySystemGlobals::TargetHasMatchingGameplayTag(GetInstigator(), FScWCoreTags::Character_Type_Player);
		return UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(SpawnParams);
	}
	return nullptr;
}
//~ End Swing FX

//~ Begin Patterns
FVector AScWWeaponActor_CommonMelee::BP_GetPatternStartLocation_Implementation(const FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex) const
{
	ensureReturn(GetInstigator(), GetActorLocation());
	return GetInstigator()->GetPawnViewLocation() + GetInstigator()->GetViewRotation().Vector() * InPatternData.TraceOffsetLocation;
}

float AScWWeaponActor_CommonMelee::BP_GetNextPatternDelayTime_Implementation(int32 InNextPatternIndex) const
{
	const float SwingDuration = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, SwingMontageSectionIndex) * SwingVariantBaseDuration;
	const auto& TracePatterns = CurrentSwingVariantData.TracePatterns;
	return (TracePatterns.Num() < 2) ? (0.0f) : (SwingDuration / (float)TracePatterns.Num());
}

void AScWWeaponActor_CommonMelee::BP_BeginTracePatterns_Implementation()
{
	ensureReturn(!CurrentSwingVariantData.TracePatterns.IsEmpty());
	BP_HandleTracePattern(CurrentSwingVariantData.TracePatterns[0], 0);
}

void AScWWeaponActor_CommonMelee::BP_HandleTracePattern_Implementation(const FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex)
{
	ensureReturn(GetInstigator());

	UWorld* World = GetWorld();
	ensureReturn(World);
	
	FVector TraceStart = BP_GetPatternStartLocation(InPatternData, InPatternIndex);
	FVector TraceEnd = TraceStart + (GetInstigator()->GetBaseAimRotation() + InPatternData.TraceOffsetRotation).Vector() * InPatternData.TraceLength;

	TArray<FHitResult> TraceHitResults;
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
	UKismetSystemLibrary::SphereTraceMulti(this, TraceStart, TraceEnd, InPatternData.TraceShapeRadius, TraceTypeQuery_Melee, false, TracePatternIgnoredActors, TracePatternDebugType, TraceHitResults, true);
	
	/*if (TraceHitResults.IsEmpty())
	{
		return;
	}*/
	for (const FHitResult& SampleHitResult : TraceHitResults)
	{
		BP_HandleSwingHit(SampleHitResult.GetActor(), SampleHitResult);
	}
	int32 NextPatternIndex = InPatternIndex + 1;
	if (CurrentSwingVariantData.TracePatterns.IsValidIndex(NextPatternIndex))
	{
		float NextPatternDelayTime = BP_GetNextPatternDelayTime(NextPatternIndex);
		if (NextPatternDelayTime > 0.0f)
		{
			FTimerDelegate NextPatternMethodDelegate;
			NextPatternMethodDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED_TwoParams(ThisClass, BP_HandleTracePattern, const FScWMeleeSwingVariantData_TracePattern&, int32), CurrentSwingVariantData.TracePatterns[NextPatternIndex], NextPatternIndex);
			World->GetTimerManager().SetTimer(NextPatternDelayHandle, NextPatternMethodDelegate, NextPatternDelayTime, false);
		}
		else
		{
			BP_HandleTracePattern(InPatternData, NextPatternIndex);
		}
	}
}
//~ End Patterns
