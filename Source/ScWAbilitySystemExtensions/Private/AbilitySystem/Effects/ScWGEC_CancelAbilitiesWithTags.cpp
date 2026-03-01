// Scientific Ways

#include "AbilitySystem/Effects/ScWGEC_CancelAbilitiesWithTags.h"

#include "AbilitySystemComponent.h"

#include "AbilitySystem/ScWGameplayAbility.h"

UScWGEC_CancelAbilitiesWithTags::UScWGEC_CancelAbilitiesWithTags()
{

}

bool UScWGEC_CancelAbilitiesWithTags::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& InContainer, FActiveGameplayEffect& InActiveEffect) const
{
	UAbilitySystemComponent* OwnerASC = InContainer.Owner;
	if (!ensure(OwnerASC))
	{
		return false;
	}
	TArray<FGameplayAbilitySpec>& AllAbilities = OwnerASC->GetActivatableAbilities();

	for (FGameplayAbilitySpec& AbilitySpec : AllAbilities)
	{
		UScWGameplayAbility* Ability = Cast<UScWGameplayAbility>(AbilitySpec.Ability);
		if (!ensure(Ability))
		{
			continue;
		}
		if ((AbilityTags.IsValid() && Ability->GetAssetTags().HasAny(AbilityTags))
			|| (CancelAbilityTags.IsValid() && Ability->GetCancelAbilitiesWithTag().HasAny(CancelAbilityTags))
			|| (BlockAbilityTags.IsValid() && Ability->GetBlockAbilitiesWithTag().HasAny(BlockAbilityTags))
			|| (ActivationOwnedTags.IsValid() && Ability->GetActivationOwnedTags().HasAny(ActivationOwnedTags))
			|| (ActivationRequiredTags.IsValid() && Ability->GetActivationRequiredTags().HasAny(ActivationRequiredTags))
			|| (ActivationBlockedTags.IsValid() && Ability->GetActivationBlockedTags().HasAny(ActivationBlockedTags)))
		{
			OwnerASC->CancelAbility(Ability);
		}
	}
	return true;
}
