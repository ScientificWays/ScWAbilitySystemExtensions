// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_ModifyCharacterMovement.h"

//~ Begin Initialize
UScWAT_ModifyCharacterMovement* UScWAT_ModifyCharacterMovement::ModifyCharacterMovement(UGameplayAbility* InOwningAbility, UCharacterMovementComponent* InTarget, FVector InMagnitude, const bool bInApplyAsForce, float InTimeOut)
{
	UScWAT_ModifyCharacterMovement* OutTaskObject = NewAbilityTask<UScWAT_ModifyCharacterMovement>(InOwningAbility);

	OutTaskObject->bTickingTask = true;
	OutTaskObject->bSimulatedTask = true;

	OutTaskObject->TargetCMC = InTarget;
	OutTaskObject->Magnitude = InMagnitude;
	OutTaskObject->bApplyAsForce = bInApplyAsForce;

	OutTaskObject->TimeOut = InTimeOut;
	return OutTaskObject;
}

void UScWAT_ModifyCharacterMovement::Activate() // UGameplayTask
{
	if (TimeOut > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeOutTimerHandle, this, &UScWAT_ModifyCharacterMovement::OnTimeOut, TimeOut, false);
	}
	Super::Activate();
}

void UScWAT_ModifyCharacterMovement::OnDestroy(bool bInAbilityIsEnding) // UGameplayTask
{
	GetWorld()->GetTimerManager().ClearTimer(TimeOutTimerHandle);
	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Tick
void UScWAT_ModifyCharacterMovement::TickTask(float InDeltaSeconds) // UGameplayTask
{
	if (!TargetCMC)
	{
		EndTask();
		return;
	}
	if (bApplyAsForce)
	{
		TargetCMC->AddForce(Magnitude);
	}
	else
	{
		TargetCMC->Velocity = Magnitude;
	}
}
//~ End Tick

//~ Begin TimeOut
void UScWAT_ModifyCharacterMovement::OnTimeOut()
{
	OnTimeOutExec.Broadcast();
	EndTask();
}
//~ End TimeOut
