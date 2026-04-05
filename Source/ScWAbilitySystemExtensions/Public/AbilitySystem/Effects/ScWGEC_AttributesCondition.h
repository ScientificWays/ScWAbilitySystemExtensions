// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "GameplayEffectComponent.h"

#include "ScWGEC_AttributesCondition.generated.h"

#define MODULE_API SCWABILITYSYSTEMEXTENSIONS_API

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
	UPROPERTY(Category = "Attributes", EditDefaultsOnly)
	FGameplayAttribute ConditionAttribute;

	UPROPERTY(Category = "Attributes", EditDefaultsOnly)
	FGameplayAttribute ConditionMaxAttribute;
};

#undef MODULE_API
