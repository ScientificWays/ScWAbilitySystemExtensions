// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "ScWAT_WaitOverlap.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskWaitOverlapDelegate, AActor*, InOverlapActor);

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] AT_WaitOverlap"))
class UScWAT_WaitOverlap : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FTaskWaitOverlapDelegate OnOverlap;
	
//~ Begin Initialize
public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (DisplayName = "[ScW] Wait for Overlap", HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitOverlap* ScWWaitOverlap(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, UClass* InOverlapClassFilter, const bool bInTriggerOnlyOnce = true);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

	UFUNCTION()
	void OnOverlapCallback(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult);

	FScriptDelegate OverlapDelegate;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> CheckComponent;

	UPROPERTY()
	TObjectPtr<UClass> OverlapClassFilter;

	UPROPERTY()
	bool bTriggerOnlyOnce;
};
