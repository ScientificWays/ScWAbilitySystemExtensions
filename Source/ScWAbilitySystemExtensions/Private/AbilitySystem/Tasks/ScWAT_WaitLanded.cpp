// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_WaitLanded.h"

//~ Begin Initialize
UScWAT_WaitLanded* UScWAT_WaitLanded::WaitLanded(UGameplayAbility* InOwningAbility, ACharacter* InTargetCharacter)
{
	UScWAT_WaitLanded* OutTaskObject = NewAbilityTask<UScWAT_WaitLanded>(InOwningAbility);
	OutTaskObject->TargetCharacter = InTargetCharacter;
	return OutTaskObject;
}

void UScWAT_WaitLanded::Activate()
{
	ensureIf(TargetCharacter)
	{
		LandedScriptDelegate.BindUFunction(this, TEXT("OnPlayerLandedCallback"));
		TargetCharacter->LandedDelegate.Add(LandedScriptDelegate);

		Super::Activate();
	}
	else
	{
		EndTask();
	}
}

void UScWAT_WaitLanded::OnDestroy(bool bInAbilityIsEnding)
{
	if (TargetCharacter)
	{
		TargetCharacter->LandedDelegate.Remove(LandedScriptDelegate);
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Task
void UScWAT_WaitLanded::OnPlayerLandedCallback(const FHitResult& InHitResult)
{
	OnPlayerLanded.Broadcast(InHitResult);
	EndTask();
}
//~ End Task
