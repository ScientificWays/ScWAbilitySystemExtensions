// Scientific Ways

#pragma once

#include "CoreMinimal.h"

#include "Abilities/Tasks/AbilityTask.h"

#include "ScWAT_MoveTarget.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Move Target"))
class UScWAT_MoveTarget : public UAbilityTask
{
	GENERATED_BODY()

	UScWAT_MoveTarget();
	
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Finished"))
	FGenericGameplayTaskDelegate OnFinished;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Time Out"))
	FGenericGameplayTaskDelegate OnTimeOut;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Target Lost"))
	FGenericGameplayTaskDelegate OnTargetLost;

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_MoveTarget* MoveComponentSweep(UGameplayAbility* InOwningAbility, USceneComponent* InAffectedComponent, const FVector& InTargetLocation, float InSpeed = 500.0f, float InArriveTolerance = 50.0f, float InTimeOut = -1.0f, bool bInApplyForce = false);
	
	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_MoveTarget* MoveComponentSweep_WithSpeedPerAxis(UGameplayAbility* InOwningAbility, USceneComponent* InAffectedComponent, const FVector& InTargetLocation, const FVector& InSpeedPerAxis = FVector(400.0f), float InArriveTolerance = 50.0f, float InTimeOut = -1.0f, bool bInApplyForce = false);

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (HidePin = "InOwningAbility", DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "true"))
	static UScWAT_MoveTarget* RotateComponentSweep(UGameplayAbility* InOwningAbility, USceneComponent* InAffectedComponent, const FRotator& InTargetRotation, float InRotateSpeed = 16.0f, float InArriveTolerance = 0.01f, float InTimeOut = -1.0f);

	//~ Begin Initialize
protected:
	virtual void Activate() override; // UGameplayTask
	virtual void OnDestroy(bool bInAbilityIsEnding) override; // UGameplayTask
	virtual void SharedInit();
//~ End Initialize

//~ Begin Tick
protected:
	virtual void TickTask(float InDeltaSeconds) override; // UGameplayTask
//~ End Tick
	
//~ Begin Simulate
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // UObject
	virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent) override;
	virtual void PreDestroyFromReplication() override; // UObject
//~ End Simulate

//~ Begin TimeOut
protected:
	void OnTimeOutCallback();
//~ End TimeOut

	UPROPERTY(Replicated)
	TObjectPtr<USceneComponent> AffectedComponent;

	UPROPERTY(Replicated)
	bool bMove;

	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	bool bRotate;

	UPROPERTY(Replicated)
	FQuat TargetQuat;

	UPROPERTY(Replicated)
	float Speed;

	UPROPERTY(Replicated)
	FVector SpeedPerAxis;

	UPROPERTY(Replicated)
	bool bUsePerAxisSpeed;

	UPROPERTY(Replicated)
	float ArriveTolerance;

	UPROPERTY(Replicated)
	bool bApplyForce;

	UPROPERTY(Replicated)
	float TimeOut;

	FTimerHandle TimeOutTimerHandle;
};
