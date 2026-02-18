// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "AbilitySystem/ScWGameplayAbility.h"

#include "ScWGameplayAbility_CommonSwing.generated.h"

#define MODULE_API SCWABILITYSYSTEMEXTENSIONS_API

class AScWWeaponActor_CommonMelee;

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Common Swing Gameplay Ability"))
class UScWGameplayAbility_CommonSwing : public UScWGameplayAbility
{

	GENERATED_BODY()

public:

	MODULE_API UScWGameplayAbility_CommonSwing();

//~ Begin Ability
public:

	UFUNCTION(Category = "Ability", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Melee Weapon Actor"))
	MODULE_API AScWWeaponActor_CommonMelee* BP_GetMeleeWeaponActor() const;

protected:
	MODULE_API virtual void ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UScWGameplayAbility
	MODULE_API virtual void BeginSwingSequence();

	UFUNCTION()
	MODULE_API void OnPreSwingDelayFinished();

	UFUNCTION()
	MODULE_API void OnSwingFinished();

	UFUNCTION()
	MODULE_API void OnPostSwingDelayFinished();

	MODULE_API virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability

//~ Begin Swing
protected:

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle PreSwing", ReturnDisplayName = "Out PreSwingDelay"))
	MODULE_API float BP_HandlePreSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle BeginSwing", ReturnDisplayName = "Out SwingDuration"))
	MODULE_API float BP_HandleBeginSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle EndSwing", ReturnDisplayName = "Out PostSwingDelay"))
	MODULE_API float BP_HandleEndSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Handle PostSwing"))
	MODULE_API void BP_HandlePostSwing();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get SwingDamage"))
	MODULE_API float BP_GetSwingDamage() const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get SwingDamageTypeClass"))
	MODULE_API TSubclassOf<class UScWDamageType> BP_GetSwingDamageTypeClass() const;

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Apply SwingOwnerEffect"))
	MODULE_API void BP_ApplySwingOwnerEffect();

	UFUNCTION(Category = "Swing", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Remove SwingOwnerEffect"))
	MODULE_API void BP_RemoveSwingOwnerEffect();

	UPROPERTY(Category = "Swing", EditAnywhere, BlueprintReadWrite)
	bool bLoopIfInputIsPressed;

	UPROPERTY(Transient)
	FActiveGameplayEffectHandle CurrentSwingEffectHandle;
//~ End Swing

//~ Begin Capture
protected:

	UPROPERTY(Category = "Capture", EditAnywhere, BlueprintReadWrite)
	bool bCaptureDamageDataOnBeginSwingSequence;

	UPROPERTY(Category = "Capture", BlueprintReadWrite)
	float CapturedSwingDamage;

	UPROPERTY(Category = "Capture", BlueprintReadWrite)
	TSubclassOf<class UScWDamageType> CapturedSwingDamageTypeClass;
//~ End Capture
};

#undef MODULE_API
