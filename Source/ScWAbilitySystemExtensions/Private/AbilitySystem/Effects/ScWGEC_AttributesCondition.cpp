// Scientific Ways

#include "AbilitySystem/Effects/ScWGEC_AttributesCondition.h"

#include "AbilitySystemLog.h"

UScWGEC_AttributesCondition::UScWGEC_AttributesCondition()
{

}

bool UScWGEC_AttributesCondition::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& InContainer, FActiveGameplayEffect& InActiveEffect) const
{
	auto OwnerASC = InContainer.Owner;
	if (!ensure(OwnerASC))
	{
		return false;
	}
	//using namespace UE::AbilitySystem::Private;
	//const bool bAllowClientSideRemoval = EnumHasAnyFlags(static_cast<EAllowPredictiveGEFlags>(CVarAllowPredictiveGEFlagsValue), EAllowPredictiveGEFlags::AllowRemovalByTagRequirements);
	//const bool bAuthority = GEContainer.IsNetAuthority();

	FActiveGameplayEffectHandle ActiveEffectHandle = InActiveEffect.Handle;
	if (FActiveGameplayEffectEvents* EventSet = OwnerASC->GetActiveEffectEventSet(ActiveEffectHandle))
	{
		FDelegateHandle	AttributesChangedHandle = OwnerASC->GetGameplayAttributeValueChangeDelegate(ConditionAttribute).AddUObject(this, &ThisClass::OnAttributeChanged, ActiveEffectHandle);
		FDelegateHandle	AttributesMaxChangedHandle = OwnerASC->GetGameplayAttributeValueChangeDelegate(ConditionMaxAttribute).AddUObject(this, &ThisClass::OnAttributeChanged, ActiveEffectHandle);
		
		// Now when this Effect is removed, we should remove all of our registered callbacks.
		TArray<TTuple<FGameplayAttribute, FDelegateHandle>> AllBoundEvents = {
			{ ConditionAttribute, AttributesChangedHandle },
			{ ConditionMaxAttribute, AttributesMaxChangedHandle }
		};
		EventSet->OnEffectRemoved.AddUObject(this, &UScWGEC_AttributesCondition::OnGameplayEffectRemoved, OwnerASC, MoveTemp(AllBoundEvents));
	}
	else
	{
		UE_LOG(LogGameplayEffects, Error, TEXT("UScWGEC_AttributesCondition::OnGameplayEffectAdded called with InActiveEffect: %s which had an invalid FActiveGameplayEffectHandle."), *InActiveEffect.GetDebugString());
	}
	return !ShouldInhibitEffect(InActiveEffect);
}

void UScWGEC_AttributesCondition::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& InRemovalInfo, UAbilitySystemComponent* InOwnerASC, TArray<TTuple<FGameplayAttribute, FDelegateHandle>> InAllBoundEvents) const
{
	for (TTuple<FGameplayAttribute, FDelegateHandle>& SamplePair : InAllBoundEvents)
	{
		const bool bSuccess = InOwnerASC->GetGameplayAttributeValueChangeDelegate(SamplePair.Key).Remove(SamplePair.Value);
		UE_CLOG(!bSuccess, LogGameplayEffects, Error, TEXT("%s tried to unregister GameplayAttributeEvent '%s' on GameplayEffect '%s' but failed."), *GetName(), *SamplePair.Key.GetName(), *GetNameSafe(GetOwner()));
	}
}

void UScWGEC_AttributesCondition::OnAttributeChanged(const FOnAttributeChangeData& InData, FActiveGameplayEffectHandle InEffectHandle) const
{
	auto OwnerASC = InEffectHandle.GetOwningAbilitySystemComponent();
	if (!ensure(OwnerASC))
	{
		return;
	}
	// It's possible for this to return nullptr if it was in the process of being removed (IsPendingRemove)
	if (const FActiveGameplayEffect* ActiveEffect = OwnerASC->GetActiveGameplayEffect(InEffectHandle))
	{
		// See if we should be inhibiting the execution
		constexpr bool bInvokeCuesIfStateChanged = true;
		const bool	   bInhibitEffect = ShouldInhibitEffect(*ActiveEffect);
		OwnerASC->SetActiveGameplayEffectInhibit(MoveTemp(InEffectHandle), bInhibitEffect, bInvokeCuesIfStateChanged);
	}
}

bool UScWGEC_AttributesCondition::ShouldInhibitEffect(const FActiveGameplayEffect& InActiveEffect) const
{
	auto OwnerASC = InActiveEffect.Handle.GetOwningAbilitySystemComponent();
	if (!ensure(OwnerASC))
	{
		return false;
	}
	auto ConditionValue = FMath::RoundToInt(OwnerASC->GetNumericAttribute(ConditionAttribute));
	auto ConditionMaxValue = FMath::RoundToInt(OwnerASC->GetNumericAttribute(ConditionMaxAttribute));

	return ConditionValue >= ConditionMaxValue;
}
