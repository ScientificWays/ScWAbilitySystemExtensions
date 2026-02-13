// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "GameplayEffectComponent.h"

#include "ScWGEC_AttributesCondition.generated.h"

/**
 *	Inhibits effect when ConditionAttribute >= ConditionMaxAttribute
 */
UCLASS(DisplayName = "[ScW] Attributes Inhibit Condition")
class UScWGEC_AttributesCondition : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:

	UScWGEC_AttributesCondition();

	
	/** Once we've applied, we need to register for ongoing requirements */
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& InContainer, FActiveGameplayEffect& InActiveEffect) const override; // UGameplayEffectComponent
private:
	/** We need to be notified when we're removed to unregister callbacks */
	void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& InRemovalInfo, UAbilitySystemComponent* InOwnerASC, TArray<TTuple<FGameplayAttribute, FDelegateHandle>> InAllBoundEvents) const;

	/** We need to register a callback for when the owner changes its attributes. When that happens, we need to figure out if our GE should inhibit */
	void OnAttributeChanged(const FOnAttributeChangeData& InData, FActiveGameplayEffectHandle InEffectHandle) const;
	bool ShouldInhibitEffect(const FActiveGameplayEffect& InActiveEffect) const;

public:

	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	FGameplayAttribute ConditionAttribute;

	UPROPERTY(EditDefaultsOnly, Category = Attributes)
	FGameplayAttribute ConditionMaxAttribute;
};
