// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "ScWAT_WaitLanded.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] AT_WaitLand"))
class UScWAT_WaitLanded : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FLandedSignature OnPlayerLanded;
	
//~ Begin Initialize
public:

	UFUNCTION(Category = "Ability | Tasks", BlueprintCallable, meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitLanded* WaitLanded(UGameplayAbility* InOwningAbility, ACharacter* InTargetCharacter);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Task
protected:

	UFUNCTION()
	void OnPlayerLandedCallback(const FHitResult& InHitResult);

	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter;

	FScriptDelegate LandedScriptDelegate;
//~ End Task
};
