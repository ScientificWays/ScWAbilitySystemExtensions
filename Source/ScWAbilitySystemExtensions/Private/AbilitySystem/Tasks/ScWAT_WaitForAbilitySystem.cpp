// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_WaitForAbilitySystem.h"

UScWAT_WaitForAbilitySystem* UScWAT_WaitForAbilitySystem::WaitForAbilitySystem(AActor* InTargetActor, float InTimeout, float InCheckInterval)
{
	UScWAT_WaitForAbilitySystem* OutTask = NewObject<UScWAT_WaitForAbilitySystem>();
 	OutTask->WatchedActor = InTargetActor;
	OutTask->TimeoutSeconds = InTimeout;
	OutTask->IntervalSeconds = InCheckInterval;

	if (!IsValid(InTargetActor))
	{
		ensure(false);
		OutTask->SetReadyToDestroy();
	}
	return OutTask;
}

void UScWAT_WaitForAbilitySystem::Activate() // UBlueprintAsyncActionBase
{
	Super::Activate();
	PollAbilitySystem(); // Start poll loop
}

void UScWAT_WaitForAbilitySystem::Cancel() // UCancellableAsyncAction
{
	if (bCompleted)
	{
		return;
	}
	bCompleted = true;

	if (WatchedActor.IsValid())
	{
		if (UWorld* World = WatchedActor->GetWorld())
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
	OnCancelled.Broadcast(nullptr);
	SetReadyToDestroy();
}

void UScWAT_WaitForAbilitySystem::Finish(UAbilitySystemComponent* InFoundASC)
{
	if (bCompleted)
	{
		return;
	}
	bCompleted = true;

	if (InFoundASC)
	{
		OnFound.Broadcast(InFoundASC);
	}
	else
	{
		OnCancelled.Broadcast(nullptr);
	}
	SetReadyToDestroy();
}

void UScWAT_WaitForAbilitySystem::PollAbilitySystem()
{
	if (bCompleted || !WatchedActor.IsValid())
	{
		Finish(nullptr);
		return;
	}
	if (UAbilitySystemComponent* WatchedActorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(WatchedActor.Get()))
	{
		Finish(WatchedActorASC);
		return;
	}
	ElapsedTime += IntervalSeconds;

	if (ElapsedTime >= TimeoutSeconds)
	{
		Finish(nullptr);
	}
	else if (UWorld* World = WatchedActor->GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &UScWAT_WaitForAbilitySystem::PollAbilitySystem, IntervalSeconds, false);
	}
}
