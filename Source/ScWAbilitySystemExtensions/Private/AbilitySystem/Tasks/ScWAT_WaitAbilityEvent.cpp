// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_WaitAbilityEvent.h"

#include "AbilitySystem/ScWCoreTags.h"

UScWAT_WaitAbilityEvent* UScWAT_WaitAbilityEvent::WaitForAbilityEvent(AActor* InTargetActor, FGameplayTag InWithTag, FGameplayTag InWithoutTag, const EScWAbilityEventType InEventType, const bool bInIncludeTriggeredAbilities, const bool bInTriggerOnce)
{
	UScWAT_WaitAbilityEvent* OutTask = NewObject<UScWAT_WaitAbilityEvent>();
	OutTask->SetAbilityActor(InTargetActor);
	OutTask->EventType = InEventType;
	OutTask->WithTag = InWithTag;
	OutTask->WithoutTag = InWithoutTag;
	OutTask->bIncludeTriggeredAbilities = bInIncludeTriggeredAbilities;
	OutTask->bTriggerOnce = bInTriggerOnce;
	return OutTask;
}

UScWAT_WaitAbilityEvent* UScWAT_WaitAbilityEvent::WaitForAbilityEventWithTagRequirements(AActor* InTargetActor, FGameplayTagRequirements InTagRequirements, const EScWAbilityEventType InEventType, const bool bInIncludeTriggeredAbilities, const bool bInTriggerOnce)
{
	UScWAT_WaitAbilityEvent* OutTask = NewObject<UScWAT_WaitAbilityEvent>();
	OutTask->SetAbilityActor(InTargetActor);
	OutTask->EventType = InEventType;
	OutTask->TagRequirements = InTagRequirements;
	OutTask->bIncludeTriggeredAbilities = bInIncludeTriggeredAbilities;
	OutTask->bTriggerOnce = bInTriggerOnce;
	return OutTask;
}

UScWAT_WaitAbilityEvent* UScWAT_WaitAbilityEvent::WaitForAbilityEvent_Query(AActor* InTargetActor, FGameplayTagQuery InQuery, const EScWAbilityEventType InEventType, const bool bInIncludeTriggeredAbilities, const bool bInTriggerOnce)
{
	UScWAT_WaitAbilityEvent* OutTask = NewObject<UScWAT_WaitAbilityEvent>();
	OutTask->SetAbilityActor(InTargetActor);
	OutTask->EventType = InEventType;
	OutTask->Query = InQuery;
	OutTask->bIncludeTriggeredAbilities = bInIncludeTriggeredAbilities;
	OutTask->bTriggerOnce = bInTriggerOnce;
	return OutTask;
}

//~ Begin Initialize
void UScWAT_WaitAbilityEvent::Activate() // UBlueprintAsyncActionBase
{
	Super::Activate();

	if (UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponent())
	{
		switch (EventType)
		{
			case EScWAbilityEventType::Activated:
			{
				EventDelegateHandle = OwnerASC->AbilityActivatedCallbacks.AddUObject(this, &UScWAT_WaitAbilityEvent::OnAbilityActivate);
				break;
			}
			case EScWAbilityEventType::Committed:
			{
				EventDelegateHandle = OwnerASC->AbilityCommittedCallbacks.AddUObject(this, &UScWAT_WaitAbilityEvent::OnAbilityCommitted);
				break;
			}
			case EScWAbilityEventType::Failed:
			{
				EventDelegateHandle = OwnerASC->AbilityFailedCallbacks.AddUObject(this, &UScWAT_WaitAbilityEvent::OnAbilityFailed);
				break;
			}
			case EScWAbilityEventType::Cancelled:
			{
				EventDelegateHandle = OwnerASC->OnAbilityEnded.AddUObject(this, &UScWAT_WaitAbilityEvent::OnAbilityEnded);
				break;
			}
			case EScWAbilityEventType::Ended:
			{
				EventDelegateHandle = OwnerASC->OnAbilityEnded.AddUObject(this, &UScWAT_WaitAbilityEvent::OnAbilityEnded);
				break;
			}
		}
	}
	else
	{
		EndAction();
	}
}

void UScWAT_WaitAbilityEvent::EndAction() // UAbilityAsync
{
	if (UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponent())
	{
		switch (EventType)
		{
			case EScWAbilityEventType::Activated:
			{
				OwnerASC->AbilityActivatedCallbacks.Remove(EventDelegateHandle);
				break;
			}
			case EScWAbilityEventType::Committed:
			{
				OwnerASC->AbilityCommittedCallbacks.Remove(EventDelegateHandle);
				break;
			}
			case EScWAbilityEventType::Failed:
			{
				OwnerASC->AbilityFailedCallbacks.Remove(EventDelegateHandle);
				break;
			}
			case EScWAbilityEventType::Cancelled:
			{
				OwnerASC->OnAbilityEnded.Remove(EventDelegateHandle);
				break;
			}
			case EScWAbilityEventType::Ended:
			{
				OwnerASC->OnAbilityEnded.Remove(EventDelegateHandle);
				break;
			}
		}
	}
	Super::EndAction();
}
//~ End Initialize

//~ Begin Event
bool UScWAT_WaitAbilityEvent::IsRelevantAbility(const UGameplayAbility* InAbility) const
{
	if (!bIncludeTriggeredAbilities && InAbility->IsTriggered())
	{
		return false;
	}
	const auto& AbilityTags = InAbility->GetAssetTags();

	if (!TagRequirements.IsEmpty())
	{
		return TagRequirements.RequirementsMet(AbilityTags);
	}
	if (WithTag.IsValid() || WithoutTag.IsValid())
	{
		return (!WithTag.IsValid() || AbilityTags.HasTag(WithTag)) && (!WithoutTag.IsValid() || !AbilityTags.HasTag(WithoutTag));
	}
	if (!Query.IsEmpty())
	{
		return Query.Matches(AbilityTags);
	}
	ensureReturn(false, false);
}

void UScWAT_WaitAbilityEvent::OnAbilityActivate(UGameplayAbility* InAbility)
{
	if (IsRelevantAbility(InAbility))
	{
		if (ShouldBroadcastDelegates())
		{
			OnTrigger.Broadcast(InAbility, InAbility->GetAssetTags());
		}
		if (bTriggerOnce)
		{
			EndAction();
		}
	}
}

void UScWAT_WaitAbilityEvent::OnAbilityCommitted(UGameplayAbility* InAbility)
{
	if (IsRelevantAbility(InAbility))
	{
		if (ShouldBroadcastDelegates())
		{
			OnTrigger.Broadcast(InAbility, InAbility->GetAssetTags());
		}
		if (bTriggerOnce)
		{
			EndAction();
		}
	}
}

void UScWAT_WaitAbilityEvent::OnAbilityFailed(const UGameplayAbility* InAbility, const FGameplayTagContainer& InFailureTags)
{
	if (IsRelevantAbility(InAbility))
	{
		if (ShouldBroadcastDelegates())
		{
			auto EventTags = InAbility->GetAssetTags();
			EventTags.AppendTags(InFailureTags);
			OnTrigger.Broadcast(const_cast<UGameplayAbility*>(InAbility), EventTags);
		}
		if (bTriggerOnce)
		{
			EndAction();
		}
	}
}

void UScWAT_WaitAbilityEvent::OnAbilityEnded(const FAbilityEndedData& InData)
{
	if (IsRelevantAbility(InData.AbilityThatEnded) && (InData.bWasCancelled == (EventType == EScWAbilityEventType::Cancelled)))
	{
		if (ShouldBroadcastDelegates())
		{
			OnTrigger.Broadcast(InData.AbilityThatEnded, FGameplayTagContainer(InData.bWasCancelled ? FScWCoreTags::Ability_Event_Cancelled : FScWCoreTags::Ability_Event_Ended));
		}
		if (bTriggerOnce)
		{
			EndAction();
		}
	}
}
//~ End Event
