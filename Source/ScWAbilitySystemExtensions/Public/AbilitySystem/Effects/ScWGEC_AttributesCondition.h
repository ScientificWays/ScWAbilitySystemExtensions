// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "GameplayEffectComponent.h"

#include "ScWGEC_AttributesCondition.generated.h"

/**
 *	Inhibits an active gameplay effect while the tracked attribute is at or above its configured limit.
 *	The component keeps the inhibit state in sync as either attribute changes on the owning ability system.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Attributes Inhibit Condition"))
class UScWGEC_AttributesCondition : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	UScWGEC_AttributesCondition();
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& InContainer, FActiveGameplayEffect& InActiveEffect) const override; // UGameplayEffectComponent

private:
	void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& InRemovalInfo, UAbilitySystemComponent* InOwnerASC, TArray<TTuple<FGameplayAttribute, FDelegateHandle>> InAllBoundEvents) const;
	void OnAttributeChanged(const FOnAttributeChangeData& InData, FActiveGameplayEffectHandle InEffectHandle) const;
	bool ShouldInhibitEffect(const FActiveGameplayEffect& InActiveEffect) const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	FGameplayAttribute ConditionAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	FGameplayAttribute ConditionMaxAttribute;
};
