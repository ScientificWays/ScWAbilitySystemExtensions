// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_WaitOverlap.h"

#include "Components/CapsuleComponent.h"

//~ Begin Initialize
UScWAT_WaitOverlap* UScWAT_WaitOverlap::ScWWaitOverlap(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, UClass* InOverlapClassFilter, const bool bInTriggerOnlyOnce)
{
	UScWAT_WaitOverlap* OutTaskObject = NewAbilityTask<UScWAT_WaitOverlap>(InOwningAbility);
	OutTaskObject->CheckComponent = InCheckComponent;
	OutTaskObject->OverlapClassFilter = InOverlapClassFilter;
	OutTaskObject->bTriggerOnlyOnce = bInTriggerOnlyOnce;
	return OutTaskObject;
}

void UScWAT_WaitOverlap::Activate()
{
	ACharacter* Character = Cast<ACharacter>(AbilitySystemComponent->GetAvatarActor());

	check(Character);

	OverlapDelegate.BindUFunction(this, TEXT("OnOverlapCallback"));

	Character->GetCapsuleComponent()->OnComponentBeginOverlap.Add(OverlapDelegate);

	Super::Activate();
}

void UScWAT_WaitOverlap::OnDestroy(bool bInAbilityIsEnding)
{
	CheckComponent->OnComponentBeginOverlap.Remove(OverlapDelegate);

	Super::OnDestroy(bInAbilityIsEnding);
}
//~ End Initialize

//~ Begin Task
void UScWAT_WaitOverlap::OnOverlapCallback(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	if (InOtherActor->IsA(OverlapClassFilter))
	{
		OnOverlap.Broadcast(InOtherActor);

		if (bTriggerOnlyOnce)
		{
			EndTask();
		}
	}
}
//~ End Task
