// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "Engine/CancellableAsyncAction.h"

#include "ScWAT_WaitForAbilitySystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScWAbilitySystemExtensionsFoundDelegate, UAbilitySystemComponent*, AbilitySystem);

/**
 * 
 */
UCLASS()
class UScWAT_WaitForAbilitySystem : public UCancellableAsyncAction
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FScWAbilitySystemExtensionsFoundDelegate OnFound;

	UPROPERTY(BlueprintAssignable)
	FScWAbilitySystemExtensionsFoundDelegate OnCancelled;

	UFUNCTION(BlueprintCallable, Category = "Ability System | Common Tasks", meta = (DisplayName = "Wait for Ability System Component", DefaultToSelf = "InTargetActor", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitForAbilitySystem* WaitForAbilitySystem(AActor* InTargetActor, float InTimeout = 30.0f, float InCheckInterval = 0.1f);
	
//~ Begin Initialize
protected:
	virtual void Activate() override; // UBlueprintAsyncActionBase
	virtual void Cancel() override; // UCancellableAsyncAction
	void Finish(UAbilitySystemComponent* InFoundASC);
//~ End Initialize

//~ Begin Settings
protected:

	UPROPERTY()
	TWeakObjectPtr<AActor> WatchedActor;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	float ElapsedTime;

	UPROPERTY()
	float TimeoutSeconds;

	UPROPERTY()
	float IntervalSeconds;

	UPROPERTY()
	bool bCompleted;

	UFUNCTION()
	void PollAbilitySystem();
//~ End Settings
};
