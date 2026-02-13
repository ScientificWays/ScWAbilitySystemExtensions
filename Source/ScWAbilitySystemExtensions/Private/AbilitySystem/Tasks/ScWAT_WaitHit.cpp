// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_WaitHit.h"

//~ Begin Initialize
UScWAT_WaitHit* UScWAT_WaitHit::WaitHit(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, TSubclassOf<AActor> InHitClassFilter)
{
	UScWAT_WaitHit* OutTaskObject = NewAbilityTask<UScWAT_WaitHit>(InOwningAbility);

	OutTaskObject->CheckComponent = InCheckComponent;
	OutTaskObject->HitClassFilter = InHitClassFilter;

	OutTaskObject->bIgnoreMovementBase = false;
	OutTaskObject->CharacterMovementComponent = nullptr;

	return OutTaskObject;
}

UScWAT_WaitHit* UScWAT_WaitHit::WaitHit_IgnoreGround(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, TSubclassOf<AActor> InHitClassFilter, UCharacterMovementComponent* InCharacterMovementComponent)
{
	UScWAT_WaitHit* OutTaskObject = NewAbilityTask<UScWAT_WaitHit>(InOwningAbility);

	OutTaskObject->CheckComponent = InCheckComponent;
	OutTaskObject->HitClassFilter = InHitClassFilter;

	OutTaskObject->bIgnoreMovementBase = true;
	OutTaskObject->CharacterMovementComponent = InCharacterMovementComponent;

	return OutTaskObject;
}

void UScWAT_WaitHit::Activate()
{
	if (CheckComponent == nullptr)
	{
		UE_LOG(LogScWAbilitySystemExtensions, Error, TEXT("%s UScWAT_WaitHit::Activate() CheckComponent == nullptr! Ending task."), *GetName());
		EndTask();
	}
	else if (bIgnoreMovementBase && CharacterMovementComponent == nullptr)
	{
		UE_LOG(LogScWAbilitySystemExtensions, Error, TEXT("%s UScWAT_WaitHit::Activate() bIgnoreMovementBase == true but CharacterMovementComponent == nullptr! Ending task."), *GetName());
		EndTask();
	}
	else
	{
		HitDelegate.BindUFunction(this, TEXT("OnHitCallback"));
		CheckComponent->OnComponentHit.Add(HitDelegate);
		Super::Activate();
	}
}

void UScWAT_WaitHit::OnDestroy(bool bInAbilityIsEnding)
{
	CheckComponent->OnComponentHit.Remove(HitDelegate);
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Hit
void UScWAT_WaitHit::OnHitCallback(UPrimitiveComponent* InHitComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, FVector InNormalImpulse, const FHitResult& InHit)
{
	check(!bIgnoreMovementBase || CharacterMovementComponent);

	if (bIgnoreMovementBase && InOtherComponent == CharacterMovementComponent->GetMovementBase())
	{
		return;
	}
	if (HitClassFilter && !InOtherActor->IsA(HitClassFilter))
	{
		return;
	}
	OnHit.Broadcast(InOtherActor);
	EndTask();
}
//~ End Hit
