// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "Abilities/Async/AbilityAsync.h"

#include "ScWAT_WaitAbilityEvent.generated.h"

UENUM(BlueprintType)
enum class EScWAbilityEventType : uint8
{
	Activated,
	Committed,
	Failed,
	Cancelled,
	Ended,
};

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScWAbilityEventSignature, UGameplayAbility*, InAbility, const FGameplayTagContainer&, InEventTags);

/**
 * 
 */
UCLASS()
class UScWAT_WaitAbilityEvent : public UAbilityAsync
{
	GENERATED_BODY()

public:

	/**
	 * If used in an ability graph, this async action will wait even after ability ends.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability System | Common Tasks", meta = (DisplayName = "Wait for Ability Event", DefaultToSelf = "InTargetActor", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitAbilityEvent* WaitForAbilityEvent(AActor* InTargetActor, FGameplayTag WithTag, FGameplayTag InWithoutTag, const EScWAbilityEventType InEventType = EScWAbilityEventType::Committed, const bool bInIncludeTriggeredAbilities = false, const bool bInTriggerOnce = true);

	/**
	 * If used in an ability graph, this async action will wait even after ability ends.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability System | Common Tasks", meta = (DisplayName = "Wait for Ability Event with Tag Requirements", DefaultToSelf = "InTargetActor", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitAbilityEvent* WaitForAbilityEventWithTagRequirements(AActor* InTargetActor, FGameplayTagRequirements InTagRequirements, const EScWAbilityEventType InEventType = EScWAbilityEventType::Committed, const bool bInIncludeTriggeredAbilities = false, const bool bInTriggerOnce = true);

	/**
	 * If used in an ability graph, this async action will wait even after ability ends.
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability System | Common Tasks", meta = (DisplayName = "Wait for Ability Event with Query", DefaultToSelf = "InTargetActor", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitAbilityEvent* WaitForAbilityEvent_Query(AActor* InTargetActor, FGameplayTagQuery InQuery, const EScWAbilityEventType InEventType = EScWAbilityEventType::Committed, const bool bInIncludeTriggeredAbilities = false, const bool bInTriggerOnce = true);
	
	UPROPERTY(BlueprintAssignable)
	FScWAbilityEventSignature OnTrigger;

//~ Begin Initialize
protected:
	virtual void Activate() override; // UBlueprintAsyncActionBase
	virtual void EndAction() override; // UAbilityAsync
//~ End Initialize

//~ Begin Event
protected:
	bool IsRelevantAbility(const UGameplayAbility* InAbility) const;

	UFUNCTION()
	void OnAbilityActivate(UGameplayAbility* InAbility);

	UFUNCTION()
	void OnAbilityCommitted(UGameplayAbility* InAbility);

	UFUNCTION()
	void OnAbilityFailed(const UGameplayAbility* InAbility, const FGameplayTagContainer& InFailureTags);

	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& InData);

	FDelegateHandle EventDelegateHandle;
//~ End Event

//~ Begin Settings
protected:

	UPROPERTY()
	EScWAbilityEventType EventType;

	UPROPERTY()
	FGameplayTag WithTag;

	UPROPERTY()
	FGameplayTag WithoutTag;

	UPROPERTY()
	bool bIncludeTriggeredAbilities;

	UPROPERTY()
	bool bTriggerOnce;

	UPROPERTY()
	FGameplayTagRequirements TagRequirements;

	UPROPERTY()
	FGameplayTagQuery Query;
//~ End Settings
};
