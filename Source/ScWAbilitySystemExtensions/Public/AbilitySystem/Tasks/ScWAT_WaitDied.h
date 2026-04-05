// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "ScWAT_WaitDied.generated.h"

#define MODULE_API SCWABILITYSYSTEMEXTENSIONS_API

/**
 *	Waits for a health component to report death and then completes the owning ability task.
 *	This is used to bridge death callbacks into a Blueprint-friendly ability task node.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] AT_WaitDied"))
class UScWAT_WaitDied : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnDied;
	
//~ Begin Initialize
public:

	UFUNCTION(Category = "Ability | Tasks", BlueprintCallable, meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitDied* WaitDied(UGameplayAbility* InOwningAbility, class UScWAttributeHealthComponent* InTargetHealthComponent);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Death
protected:

	UFUNCTION()
	void OnDiedCallback();

	UPROPERTY()
	TObjectPtr<class UScWAttributeHealthComponent> TargetHealthComponent;

	FScriptDelegate OnDiedScriptDelegate;
//~ End Death
};

#undef MODULE_API
