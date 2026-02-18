// Scientific Ways

#include "AbilitySystem/Tasks/ScWAT_MoveTarget.h"

#include "Net/UnrealNetwork.h"

//UE_DISABLE_OPTIMIZATION

FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit)
{
	return FVector::VectorPlaneProject(Delta, Normal) * Time;
}

void TwoWallAdjust(FVector& OutDelta, const FHitResult& Hit, const FVector& OldHitNormal)
{
	FVector		  Delta = OutDelta;
	const FVector HitNormal = Hit.Normal;

	if ((OldHitNormal | HitNormal) <= 0.f) // 90 or less corner, so use cross product for direction
	{
		const FVector DesiredDir = Delta;
		FVector		  NewDir = (HitNormal ^ OldHitNormal);
		NewDir = NewDir.GetSafeNormal();
		Delta = (Delta | NewDir) * (1.f - Hit.Time) * NewDir;
		if ((DesiredDir | Delta) < 0.f)
		{
			Delta = -1.f * Delta;
		}
	}
	else // adjust to new wall
	{
		const FVector DesiredDir = Delta;
		Delta = ComputeSlideVector(Delta, 1.f - Hit.Time, HitNormal, Hit);
		if ((Delta | DesiredDir) <= 0.f)
		{
			Delta = FVector::ZeroVector;
		}
		else if (FMath::Abs((HitNormal | OldHitNormal) - 1.f) < KINDA_SMALL_NUMBER)
		{
			// we hit the same wall again even after adjusting to move along it the first time
			// nudge away from it (this can happen due to precision issues)
			Delta += HitNormal * 0.01f;
		}
	}

	OutDelta = Delta;
}

UScWAT_MoveTarget::UScWAT_MoveTarget()
{
	bTickingTask = true;
	bSimulatedTask = true;
}

UScWAT_MoveTarget* UScWAT_MoveTarget::MoveComponentSweep(UGameplayAbility* InOwningAbility, USceneComponent* InAffectedComponent, const FVector& InTargetLocation, float InSpeed, float InArriveTolerance, float InTimeOut, bool bInApplyForce)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);

	OutTaskObject->AffectedComponent = InAffectedComponent;

	OutTaskObject->bMove = true;
	OutTaskObject->bRotate = false;
	OutTaskObject->TargetLocation = InTargetLocation;
	OutTaskObject->Speed = InSpeed;
	OutTaskObject->bUsePerAxisSpeed = false;

	OutTaskObject->ArriveTolerance = InArriveTolerance;
	OutTaskObject->TimeOut = InTimeOut;
	OutTaskObject->bApplyForce = bInApplyForce;

	return OutTaskObject;
}

UScWAT_MoveTarget* UScWAT_MoveTarget::MoveComponentSweep_WithSpeedPerAxis(UGameplayAbility* InOwningAbility, USceneComponent* InAffectedComponent, const FVector& InTargetLocation, const FVector& InSpeedPerAxis, float InArriveTolerance, float InTimeOut, bool bInApplyForce)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);

	OutTaskObject->AffectedComponent = InAffectedComponent;

	OutTaskObject->bMove = true;
	OutTaskObject->bRotate = false;
	OutTaskObject->TargetLocation = InTargetLocation;
	OutTaskObject->SpeedPerAxis = InSpeedPerAxis;
	OutTaskObject->bUsePerAxisSpeed = true;

	OutTaskObject->ArriveTolerance = InArriveTolerance;
	OutTaskObject->TimeOut = InTimeOut;
	OutTaskObject->bApplyForce = bInApplyForce;

	return OutTaskObject;
}

UScWAT_MoveTarget* UScWAT_MoveTarget::RotateComponentSweep(UGameplayAbility* InOwningAbility, USceneComponent* InAffectedComponent, const FRotator& InTargetRotation, float InRotateSpeed, float InArriveTolerance, float InTimeOut)
{
	ThisClass* OutTaskObject = NewAbilityTask<ThisClass>(InOwningAbility);

	OutTaskObject->AffectedComponent = InAffectedComponent;

	OutTaskObject->bMove = false;
	OutTaskObject->bRotate = true;
	OutTaskObject->TargetQuat = FQuat(InTargetRotation);
	OutTaskObject->Speed = InRotateSpeed;

	OutTaskObject->ArriveTolerance = InArriveTolerance;
	OutTaskObject->TimeOut = InTimeOut;

	return OutTaskObject;
}

//~ Begin Initialize
void UScWAT_MoveTarget::Activate() // UGameplayTask
{
	if (AffectedComponent == nullptr)
	{
		OnTargetLost.Broadcast();
		EndTask();
		return;
	}
	SharedInit();

	Super::Activate();
}

void UScWAT_MoveTarget::OnDestroy(bool bInAbilityIsEnding) // UGameplayTask
{
	GetWorld()->GetTimerManager().ClearTimer(TimeOutTimerHandle);
	Super::OnDestroy(bInAbilityIsEnding);
}

void UScWAT_MoveTarget::SharedInit()
{
	if (TimeOut > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TimeOutTimerHandle, this, &ThisClass::OnTimeOutCallback, TimeOut, false);
	}
}
//~ End Initialize

//UE_DISABLE_OPTIMIZATION

//~ Begin Tick
void UScWAT_MoveTarget::TickTask(float InDeltaSeconds) // UGameplayTask
{
	if (!IsValid(AffectedComponent))
	{
		OnTargetLost.Broadcast();
		EndTask();
		return;
	}
	if (bRotate)
	{
		FQuat CurrentQuat = AffectedComponent->GetComponentQuat();
		FQuat NewQuat = TargetQuat;

		float Alpha = Speed * InDeltaSeconds;
		if (Alpha < 1.0f)
		{
			NewQuat = FQuat::Slerp(CurrentQuat, TargetQuat, Alpha);
		}
		FHitResult MoveHitResult;
		AffectedComponent->MoveComponent(FVector::ZeroVector, NewQuat, true, &MoveHitResult);

		if (AffectedComponent->GetComponentQuat().Equals(TargetQuat, ArriveTolerance))
		{
			OnFinished.Broadcast();
			EndTask();
		}
	}
	if (bMove)
	{
		FVector CurrentLocation = AffectedComponent->GetComponentLocation();
		FVector DistanceVector = TargetLocation - CurrentLocation;
		FVector Delta = FVector::ZeroVector;

		if (bUsePerAxisSpeed)
		{
			Delta = SpeedPerAxis * InDeltaSeconds;
			Delta.X = DistanceVector.X < 0.0f ? -FMath::Max(Delta.X, DistanceVector.X) : FMath::Min(Delta.X, DistanceVector.X);
			Delta.Y = DistanceVector.Y < 0.0f ? -FMath::Max(Delta.Y, DistanceVector.Y) : FMath::Min(Delta.Y, DistanceVector.Y);
			Delta.Z = DistanceVector.Z < 0.0f ? -FMath::Max(Delta.Z, DistanceVector.Z) : FMath::Min(Delta.Z, DistanceVector.Z);
		}
		else
		{
			float MoveDistance = Speed * InDeltaSeconds;
			float DistanceLeft = DistanceVector.Size();

			FVector MoveDirection = DistanceVector;
			MoveDirection.Normalize();
			Delta = MoveDirection * FMath::Min(MoveDistance, DistanceLeft);
		}
		if (bApplyForce)
		{
			Cast<UPrimitiveComponent>(AffectedComponent)->AddForce(Delta, NAME_None, true);
		}
		else
		{
			FHitResult MoveHitResult;
			if (!AffectedComponent->MoveComponent(Delta, AffectedComponent->GetComponentQuat(), true, &MoveHitResult))
			{
				// Slide along surfaces
				FVector OldHitNormal = MoveHitResult.Normal;
				FVector AdjustedDelta = Delta;
				TwoWallAdjust(AdjustedDelta, MoveHitResult, OldHitNormal);
				AffectedComponent->MoveComponent(AdjustedDelta, AffectedComponent->GetComponentQuat(), true, &MoveHitResult);
			}
		}
		if (AffectedComponent->GetComponentLocation().Equals(TargetLocation, ArriveTolerance))
		{
			OnFinished.Broadcast();
			EndTask();
		}
	}
}
//~ End Tick

//~ Begin Simulate
void UScWAT_MoveTarget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // UObject
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AffectedComponent);
	DOREPLIFETIME(ThisClass, bMove);
	DOREPLIFETIME(ThisClass, TargetLocation);
	DOREPLIFETIME(ThisClass, bRotate);
	DOREPLIFETIME(ThisClass, TargetQuat);
	DOREPLIFETIME(ThisClass, Speed);
	DOREPLIFETIME(ThisClass, SpeedPerAxis);
	DOREPLIFETIME(ThisClass, bUsePerAxisSpeed);
	DOREPLIFETIME(ThisClass, ArriveTolerance);
	DOREPLIFETIME(ThisClass, bApplyForce);
	DOREPLIFETIME(ThisClass, TimeOut);
}

void UScWAT_MoveTarget::InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent)
{
	Super::InitSimulatedTask(InGameplayTasksComponent);

	SharedInit();
}

void UScWAT_MoveTarget::PreDestroyFromReplication() // UObject
{
	EndTask();
}
//~ End Simulate

//~ Begin TimeOut
void UScWAT_MoveTarget::OnTimeOutCallback()
{
	OnTimeOut.Broadcast();
	EndTask();
}
//~ End TimeOut

//UE_ENABLE_OPTIMIZATION
