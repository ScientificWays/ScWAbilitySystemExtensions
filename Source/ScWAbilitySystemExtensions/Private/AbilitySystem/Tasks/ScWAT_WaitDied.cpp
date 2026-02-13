// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_WaitDied.h"

#include "Damage/ScWAttributeHealthComponent.h"

//~ Begin Initialize
UScWAT_WaitDied* UScWAT_WaitDied::WaitDied(UGameplayAbility* InOwningAbility, UScWAttributeHealthComponent* InTargetHealthComponent)
{
	UScWAT_WaitDied* OutTaskObject = NewAbilityTask<UScWAT_WaitDied>(InOwningAbility);
	OutTaskObject->TargetHealthComponent = InTargetHealthComponent;
	return OutTaskObject;
}

void UScWAT_WaitDied::Activate()
{
	ensure(TargetHealthComponent);
	if (TargetHealthComponent)
	{
		OnDiedScriptDelegate.BindUFunction(this, TEXT("OnDiedCallback"));
		TargetHealthComponent->OnDied.Add(OnDiedScriptDelegate);
		Super::Activate();
	}
	else
	{
		EndTask();
	}
}

void UScWAT_WaitDied::OnDestroy(bool bInAbilityIsEnding)
{
	ensure(TargetHealthComponent);
	if (TargetHealthComponent)
	{
		TargetHealthComponent->OnDied.Remove(OnDiedScriptDelegate);
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Death
void UScWAT_WaitDied::OnDiedCallback()
{
	OnDied.Broadcast();
	EndTask();
}
//~ End Death
