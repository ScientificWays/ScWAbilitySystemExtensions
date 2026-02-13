// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "ScWAT_WaitDied.generated.h"

/**
 * 
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

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
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
