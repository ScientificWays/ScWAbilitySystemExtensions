// Scientific Ways

#pragma once

#include "CoreMinimal.h"

#include "GameplayEffect.h"
#include "GameplayEffectComponent.h"

#include "ScWGEC_CancelAbilitiesWithTags.generated.h"

#define MODULE_API SCWABILITYSYSTEMEXTENSIONS_API

/**
 *	Cancels active abilities that match the configured tag filters when the owning gameplay effect is applied.
 *	The component inspects asset, cancel, block, and activation tag sets on each activatable ability.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Cancel Abilities With Tags"))
class UScWGEC_CancelAbilitiesWithTags : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	UScWGEC_CancelAbilitiesWithTags();

protected:
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& InContainer, FActiveGameplayEffect& InActiveEffect) const override; // UGameplayEffectComponent

public:
	UPROPERTY(Category = "Tags", EditDefaultsOnly)
	FGameplayTagContainer AbilityTags;

	UPROPERTY(Category = "Tags", EditDefaultsOnly)
	FGameplayTagContainer CancelAbilityTags;

	UPROPERTY(Category = "Tags", EditDefaultsOnly)
	FGameplayTagContainer BlockAbilityTags;

	UPROPERTY(Category = "Tags", EditDefaultsOnly)
	FGameplayTagContainer ActivationOwnedTags;

	UPROPERTY(Category = "Tags", EditDefaultsOnly)
	FGameplayTagContainer ActivationRequiredTags;

	UPROPERTY(Category = "Tags", EditDefaultsOnly)
	FGameplayTagContainer ActivationBlockedTags;
};

#undef MODULE_API
