// Scientific Ways

#include "AbilitySystem/Abilities/ScWGameplayAbility_CommonJump.h"

#include "AbilitySystem/ScWCoreTags.h"
#include "AbilitySystem/Tasks/ScWAT_WaitLanded.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

UScWGameplayAbility_CommonJump::UScWGameplayAbility_CommonJump()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWCoreTags::Ability_Movement_Jump);
		SetAssetTags(Tags);
	}
	CancelAbilitiesWithTag.AddTag(FScWCoreTags::Ability_CancelBy_JumpActivate);
	BlockAbilitiesWithTag.AddTag(FScWCoreTags::Ability_CancelBy_JumpActivate);

	ActivationOwnedTags.AddTag(FScWCoreTags::State_Jumping);

	bStopJumpingOnAbilityEnd = true;
}

//~ Begin Ability
void UScWGameplayAbility_CommonJump::ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UGameplayAbility
{
	Super::ActivateAbility(InHandle, InActorInfo, InActivationInfo, InTriggerEventData);

	if (CommitAbility(InHandle, InActorInfo, InActivationInfo) == false)
	{
		CancelAbility(InHandle, InActorInfo, InActivationInfo, true);
		return;
	}
	ACharacter* AvatarCharacter = GetCharacterFromActorInfo();
	ensureReturn(AvatarCharacter);
	AvatarCharacter->Jump();

	UScWAT_WaitLanded* WaitLandedTask = UScWAT_WaitLanded::WaitLanded(this, AvatarCharacter);
	WaitLandedTask->OnPlayerLanded.AddDynamic(this, &ThisClass::OnPlayerLandedCallback);
	WaitLandedTask->ReadyForActivation();

	UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnAbilityInputReleaseCallback);
	WaitInputReleaseTask->ReadyForActivation();
}

void UScWGameplayAbility_CommonJump::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	if (bStopJumpingOnAbilityEnd)
	{
		ACharacter* AvatarCharacter = GetCharacterFromActorInfo();
		ensureReturn(AvatarCharacter);
		AvatarCharacter->StopJumping();
	}
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);
}
//~ End Ability

//~ Begin Jump
void UScWGameplayAbility_CommonJump::OnPlayerLandedCallback(const FHitResult& InHitResult)
{
	K2_EndAbility();
}

void UScWGameplayAbility_CommonJump::OnAbilityInputReleaseCallback(float InTimeHeld)
{
	K2_EndAbility();
}
//~ End Jump
