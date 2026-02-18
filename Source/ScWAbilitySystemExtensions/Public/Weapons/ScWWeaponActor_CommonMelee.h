// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "Animation/ScWTypes_Animation.h"

#include "ScWWeaponActor_CommonMelee.generated.h"

#define MODULE_API SCWABILITYSYSTEMEXTENSIONS_API

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScWMeleeWeaponPreSwingSignature, float, InPreSwingDelay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScWMeleeWeaponBeginSwingSignature, float, InSwingDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScWMeleeWeaponEndSwingSignature, float, InEndSwingDelay, const bool, bInWasCancelled);

UENUM(BlueprintType, meta = (DisplayName = "[ScW] Ability Input ID"))
enum class EScWSwingPhase : uint8
{
	None,
	PreSwing,
	Swing,
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Melee Swing Variant Data: Trace Pattern"))
struct FScWMeleeSwingVariantData_TracePattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TraceOffsetRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TraceOffsetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceLength = 128.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceShapeRadius = 32.0f;
};

USTRUCT(BlueprintType, meta = (DisplayName = "[ScW] Melee Swing Variant Data"))
struct FScWMeleeSwingVariantData
{
	GENERATED_BODY()

	// Can be empty if no traces needed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FScWMeleeSwingVariantData_TracePattern> TracePatterns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScWCharacterMontageData MontageData;

	static const FScWMeleeSwingVariantData Invalid;
};

/**
 *
 */
UCLASS(MinimalAPI, Abstract, Blueprintable, meta = (DisplayName = "[ScW] Weapon Actor (Melee)"))
class AScWWeaponActor_CommonMelee : public AActor
{
	GENERATED_BODY()

public:

	AScWWeaponActor_CommonMelee(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());
	
//~ Begin Initialize
protected:
	virtual void OnConstruction(const FTransform& InTransform) override; // AActor
	virtual void BeginPlay() override; // AActor
//~ End Initialize
	
//~ Begin Components
public:

	UFUNCTION(Category = "Components", BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "Get Collision Component"))
	MODULE_API UShapeComponent* BP_GetCollisionComponent() const;

protected:

	UFUNCTION()
	void OnCollisionComponentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepHitResult);
//~ End Components

//~ Begin Swing
public:

	UFUNCTION(Category = "Swing", BlueprintCallable)
	MODULE_API int32 GetSwingCounter() const { return SwingCounter; }

	UFUNCTION(Category = "Swing", BlueprintCallable)
	MODULE_API EScWSwingPhase GetCurrentSwingPhase() const { return CurrentSwingPhase; }

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get SwingDamage"))
	MODULE_API float BP_GetSwingDamage() const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Swing Damage Type Class"))
	MODULE_API TSubclassOf<class UScWDamageType> BP_GetSwingDamageTypeClass() const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Swing Variants"))
	MODULE_API void BP_GetSwingVariants(TArray<FScWMeleeSwingVariantData>& OutVariants) const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Pre Swing"))
	MODULE_API float BP_PreSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Begin Swing"))
	MODULE_API float BP_BeginSwing(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass);

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "End Swing"))
	MODULE_API float BP_EndSwing(const bool bInWasCancelled);

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Reset Swing Components"))
	MODULE_API void BP_ResetSwingComponents();

	UPROPERTY(Category = "Swing", BlueprintAssignable)
	FScWMeleeWeaponPreSwingSignature OnPreSwingDelegate;

	UPROPERTY(Category = "Swing", BlueprintAssignable)
	FScWMeleeWeaponBeginSwingSignature OnBeginSwingDelegate;

	UPROPERTY(Category = "Swing", BlueprintAssignable)
	FScWMeleeWeaponEndSwingSignature OnEndSwingDelegate;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float SwingBaseDamage;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UScWDamageType> SwingBaseDamageTypeClass;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	float SwingAIMaxRange;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadOnly)
	FName SwingAIMaxRange_BlackboardKeyName;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadWrite)
	int32 PreSwingMontageSectionIndex;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadWrite)
	int32 SwingMontageSectionIndex;

	UPROPERTY(Category = "Swing", EditDefaultsOnly, BlueprintReadWrite)
	int32 EndSwingMontageSectionIndex;

protected:

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle Swing Hit"))
	bool BP_HandleSwingHit(AActor* InHitActor, const FHitResult& InHitResult);

	UPROPERTY(Category = "Swing", BlueprintReadOnly)
	int32 SwingCounter;

	UPROPERTY(Category = "Swing", BlueprintReadOnly)
	EScWSwingPhase CurrentSwingPhase;

	UPROPERTY(Category = "Swing", BlueprintReadOnly)
	float LastSwingDamage;

	UPROPERTY(Category = "Swing", BlueprintReadOnly)
	TSubclassOf<UDamageType> LastSwingDamageTypeClass;

	UPROPERTY(Category = "Swing", BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> LastSwingAffectedActorArray;

	UPROPERTY(Transient)
	FScWMeleeSwingVariantData CurrentSwingVariantData;
//~ End Swing

//~ Begin Swing Variants
public:

	UFUNCTION(Category = "Swing Variants", BlueprintCallable)
	MODULE_API const FScWMeleeSwingVariantData& GetCurrentSwingVariantData() const { return CurrentSwingVariantData; }

protected:

	UFUNCTION(Category = "Swing Variants", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Update Current Swing Variant Data"))
	void BP_UpdateCurrentSwingVariantData();

	UPROPERTY(Category = "Swing Variants", EditDefaultsOnly, BlueprintReadOnly)
	TArray<FScWMeleeSwingVariantData> SwingVariants;

	UPROPERTY(Category = "Swing Variants", EditDefaultsOnly, BlueprintReadOnly)
	float SwingVariantBaseDuration;
//~ End Swing Variants

//~ Begin Swing FX
protected:

	UFUNCTION(Category = "Swing FX", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get FX Attach Component"))
	USceneComponent* BP_GetFXAttachComponent() const;

	UFUNCTION(Category = "Swing FX", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Initialize Swing Particles"))
	UNiagaraComponent* BP_InitializeSwingParticles() const;

	UPROPERTY(Category = "Swing FX", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> DefaultSwingParticles;

	UPROPERTY(Category = "Swing FX", EditDefaultsOnly, BlueprintReadOnly)
	FName SwingParticlesAttachmentSocketName;

	UPROPERTY(Category = "Swing FX", EditDefaultsOnly, BlueprintReadOnly)
	FTransform SwingParticlesRelativeTransform;

	UPROPERTY(Category = "Swing FX", Transient, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> SwingPaticlesComponent;
//~ End Swing FX

//~ Begin Patterns
public:

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get PatternStart Location"))
	MODULE_API FVector BP_GetPatternStartLocation(const struct FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex) const;

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get NextPattern Delay Time"))
	MODULE_API float BP_GetNextPatternDelayTime(int32 InNextPatternIndex) const;

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Begin Trace Patterns"))
	MODULE_API void BP_BeginTracePatterns();

protected:

	UFUNCTION(Category = "Patterns", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle Trace Pattern"))
	void BP_HandleTracePattern(const struct FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex);

	UPROPERTY(Category = "Patterns", BlueprintReadWrite)
	TSet<TObjectPtr<AActor>> SwingHitIgnoredActors;

	UPROPERTY(Category = "Patterns", BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> TracePatternIgnoredActors;

	UPROPERTY(Transient)
	FTimerHandle NextPatternDelayHandle;
//~ End Patterns

//~ Begin Debug
public:

	UPROPERTY(Category = "Debug", EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<EDrawDebugTrace::Type> TracePatternDebugType;
//~ End Debug
};

#undef MODULE_API
