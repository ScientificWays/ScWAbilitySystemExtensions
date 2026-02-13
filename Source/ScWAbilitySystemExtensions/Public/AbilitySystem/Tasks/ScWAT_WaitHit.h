// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "ScWAT_WaitHit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskWaitHitSignature, AActor*, InHitActor);

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] AT_WaitHit"))
class UScWAT_WaitHit : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FTaskWaitHitSignature OnHit;
	
//~ Begin Initialize
public:

	UFUNCTION(Category = "Ability | Tasks", BlueprintCallable, meta = (DisplayName = "[ScW] Wait for Hit", HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitHit* WaitHit(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, TSubclassOf<AActor> InHitClassFilter);

	UFUNCTION(Category = "Ability | Tasks", BlueprintCallable, meta = (DisplayName = "[ScW] Wait for Hit (Ignore Ground)", HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_WaitHit* WaitHit_IgnoreGround(UGameplayAbility* InOwningAbility, UPrimitiveComponent* InCheckComponent, TSubclassOf<AActor> InHitClassFilter, UCharacterMovementComponent* InCharacterMovementComponent);

protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
//~ End Initialize

//~ Begin Hit
protected:

	UFUNCTION()
	void OnHitCallback(UPrimitiveComponent* InHitComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, FVector InNormalImpulse, const FHitResult& InHit);

	FScriptDelegate HitDelegate;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> CheckComponent;

	UPROPERTY()
	TSubclassOf<AActor> HitClassFilter;
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	bool bIgnoreMovementBase;
//~ End Hit
};
