// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "AbilitySystem/ScWGameplayAbility.h"

#include "ScWGameplayAbility_CommonJump.generated.h"

#define MODULE_API SCWGAMECORE_API

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Common Jump Gameplay Ability"))
class UScWGameplayAbility_CommonJump : public UScWGameplayAbility
{

	GENERATED_BODY()

public:

	UScWGameplayAbility_CommonJump();

//~ Begin Ability
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) override; // UGameplayAbility
	virtual void EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) override; // UGameplayAbility
//~ End Ability

//~ Begin Jump
public:

	UPROPERTY(Category = "Jump", EditAnywhere, BlueprintReadWrite)
	bool bStopJumpingOnAbilityEnd;

protected:

	UFUNCTION()
	void OnPlayerLandedCallback(const FHitResult& InHitResult);

	UFUNCTION()
	void OnAbilityInputReleaseCallback(float InTimeHeld);
//~ End Jump
};

#undef MODULE_API
