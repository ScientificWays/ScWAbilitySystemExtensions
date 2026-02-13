// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_WaitDamageBlock.h"

#include "Damage/ScWAttributeHealthComponent.h"

#include "Damage/ScWDamageType.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWAT_WaitDamageBlock)

//~ Begin Initialize
UScWAT_WaitDamageBlock* UScWAT_WaitDamageBlock::WaitDamageBlock(UGameplayAbility* InOwningAbility, UScWAttributeHealthComponent* InTargetHealthComponent, TSubclassOf<UDamageType> InDamageTypeClass, const bool bOnce)
{
	UScWAT_WaitDamageBlock* OutTaskObject = NewAbilityTask<UScWAT_WaitDamageBlock>(InOwningAbility);

	OutTaskObject->TargetHealthComponent = InTargetHealthComponent;
	OutTaskObject->bOnce = bOnce;
	OutTaskObject->DamageTypeClass = InDamageTypeClass;

	return OutTaskObject;
}

void UScWAT_WaitDamageBlock::Activate()
{
	ensure(TargetHealthComponent);
	if (TargetHealthComponent)
	{
		TargetHealthComponent->OnDamageBlocked.AddDynamic(this, &UScWAT_WaitDamageBlock::OnDamageBlockedCallback);
		Super::Activate();
	}
	else
	{
		EndTask();
	}
}

void UScWAT_WaitDamageBlock::OnDestroy(bool bInAbilityIsEnding)
{
	ensure(TargetHealthComponent);
	if (TargetHealthComponent)
	{
		TargetHealthComponent->OnDamageBlocked.RemoveDynamic(this, &UScWAT_WaitDamageBlock::OnDamageBlockedCallback);
	}
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Task
void UScWAT_WaitDamageBlock::OnDamageBlockedCallback(float InDamage, const FReceivedDamageData& InData)
{
	if (!DamageTypeClass || (InData.DamageType && InData.DamageType->IsA(DamageTypeClass)))
	{
		OnDamageBlocked.Broadcast();

		if (bOnce)
		{
			EndTask();
		}
	}
}
//~ End Task
