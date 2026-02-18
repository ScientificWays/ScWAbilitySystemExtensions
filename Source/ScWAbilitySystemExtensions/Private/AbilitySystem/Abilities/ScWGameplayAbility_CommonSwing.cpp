// Scientific Ways

#include "AbilitySystem/Abilities/ScWGameplayAbility_CommonSwing.h"

#include "AbilitySystem/Tasks/ScWAT_WaitDelay.h"
#include "AbilitySystem/ScWAbilitySystemGlobals.h"

#include "Tags/ScWAbilitySystemExtensionsTags.h"

#include "Damage/ScWDamageType.h"

#include "Character/ScWCharacter.h"

#include "Weapons/ScWWeaponActor_CommonMelee.h"

UScWGameplayAbility_CommonSwing::UScWGameplayAbility_CommonSwing()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWAbilitySystemExtensionsTags::Ability_Type_Weapon_Swing);
		SetAssetTags(Tags);
	}
	ActivationOwnedTags.AddTag(FScWAbilitySystemExtensionsTags::Character_State_Swinging);

	bCaptureDamageDataOnBeginSwingSequence = true;

	bLoopIfInputIsPressed = true;
}

//~ Begin Ability
AScWWeaponActor_CommonMelee* UScWGameplayAbility_CommonSwing::BP_GetMeleeWeaponActor_Implementation() const
{
	AScWWeaponActor_CommonMelee* WeaponActor = Cast<AScWWeaponActor_CommonMelee>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo));
	ensureReturn(WeaponActor, nullptr);
	return WeaponActor;
}

void UScWGameplayAbility_CommonSwing::ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UScWGameplayAbility
{
	ensureCancelAbilityReturn(InActorInfo);

	ACharacter* OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	ensureCancelAbilityReturn(OwnerCharacter);

	if (CommitAbility(InHandle, InActorInfo, InActivationInfo))
	{
		BeginSwingSequence();
	}
	else
	{
		K2_CancelAbility();
	}
}

void UScWGameplayAbility_CommonSwing::BeginSwingSequence()
{
	if (bCaptureDamageDataOnBeginSwingSequence)
	{
		CapturedSwingDamage = BP_GetSwingDamage();
		CapturedSwingDamageTypeClass = BP_GetSwingDamageTypeClass();
	}
	float PreSwingDelay = BP_HandlePreSwing();

	COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(PreSwingWaitDelayTask, PreSwingDelay, OnPreSwingDelayFinished);
}

void UScWGameplayAbility_CommonSwing::OnPreSwingDelayFinished()
{
	float SwingDuration = BP_HandleBeginSwing();

	COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(PatternsWaitDelayTask, SwingDuration, OnSwingFinished);
}

void UScWGameplayAbility_CommonSwing::OnSwingFinished()
{
	float PostSwingDelay = BP_HandleEndSwing();

	if (bLoopIfInputIsPressed && IsAbilityInputPressed())
	{
		BeginSwingSequence();
	}
	else
	{
		COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(PostSwingWaitDelayTask, PostSwingDelay, OnPostSwingDelayFinished);
	}
}

void UScWGameplayAbility_CommonSwing::OnPostSwingDelayFinished()
{
	BP_HandlePostSwing();
}

void UScWGameplayAbility_CommonSwing::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);

	if (CurrentSwingEffectHandle.IsValid())
	{
		BP_RemoveSwingOwnerEffect();
	}
	AScWWeaponActor_CommonMelee* WeaponActor = BP_GetMeleeWeaponActor();
	ensureReturn(WeaponActor);
	if (WeaponActor->GetCurrentSwingPhase() != EScWSwingPhase::None)
	{
		WeaponActor->BP_EndSwing(bInWasCancelled);
	}
}
//~ End Ability

//~ Begin Swing
float UScWGameplayAbility_CommonSwing::BP_HandlePreSwing_Implementation()
{
	AScWWeaponActor_CommonMelee* WeaponActor = BP_GetMeleeWeaponActor();
	ensureCancelAbilityReturn(WeaponActor, 0.0f);

	float OutPreSwingDelay = WeaponActor->BP_PreSwing();
	ensureCancelAbilityReturn(OutPreSwingDelay >= 0.0f, 0.0f);

	if (WeaponActor->SwingOwnerEffect && !WeaponActor->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_ApplySwingOwnerEffect();
	}
	return OutPreSwingDelay;
}

float UScWGameplayAbility_CommonSwing::BP_HandleBeginSwing_Implementation()
{
	AScWWeaponActor_CommonMelee* WeaponActor = BP_GetMeleeWeaponActor();
	ensureCancelAbilityReturn(WeaponActor, 0.0f);

	float OutSwingDuration = 0.0f;

	if (bCaptureDamageDataOnBeginSwingSequence)
	{
		OutSwingDuration = WeaponActor->BP_BeginSwing(CapturedSwingDamage, CapturedSwingDamageTypeClass);
	}
	else
	{
		OutSwingDuration = WeaponActor->BP_BeginSwing(BP_GetSwingDamage(), BP_GetSwingDamageTypeClass());
	}
	ensureCancelAbilityReturn(OutSwingDuration >= 0.0f, 0.0f);

	if (WeaponActor->SwingOwnerEffect && WeaponActor->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_ApplySwingOwnerEffect();
	}
	return OutSwingDuration;
}

float UScWGameplayAbility_CommonSwing::BP_HandleEndSwing_Implementation()
{
	AScWWeaponActor_CommonMelee* WeaponActor = BP_GetMeleeWeaponActor();
	ensureCancelAbilityReturn(WeaponActor, 0.0f);

	float OutPostSwingDelay = WeaponActor->BP_EndSwing(false);
	ensureCancelAbilityReturn(OutPostSwingDelay >= 0.0f, 0.0f);

	if (CurrentSwingEffectHandle.IsValid() && WeaponActor->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_RemoveSwingOwnerEffect();
	}
	return OutPostSwingDelay;
}

void UScWGameplayAbility_CommonSwing::BP_HandlePostSwing_Implementation()
{
	AScWWeaponActor_CommonMelee* WeaponActor = BP_GetMeleeWeaponActor();
	ensureCancelAbilityReturn(WeaponActor);

	if (CurrentSwingEffectHandle.IsValid() && !WeaponActor->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_RemoveSwingOwnerEffect();
	}
	K2_EndAbility();
}

float UScWGameplayAbility_CommonSwing::BP_GetSwingDamage_Implementation() const
{
	AScWWeaponActor_CommonMelee* WeaponActor = BP_GetMeleeWeaponActor();
	ensureReturn(WeaponActor, 0.0f);
	return WeaponActor->BP_GetSwingDamage();
}

TSubclassOf<UScWDamageType> UScWGameplayAbility_CommonSwing::BP_GetSwingDamageTypeClass_Implementation() const
{
	AScWWeaponActor_CommonMelee* WeaponActor = BP_GetMeleeWeaponActor();
	ensureReturn(WeaponActor, UScWDamageType::StaticClass());
	return WeaponActor->BP_GetSwingDamageTypeClass();
}

void UScWGameplayAbility_CommonSwing::BP_ApplySwingOwnerEffect_Implementation()
{
	AScWWeaponActor_CommonMelee* WeaponActor = BP_GetMeleeWeaponActor();
	ensureCancelAbilityReturn(WeaponActor);
	ensureCancelAbilityReturn(WeaponActor->SwingOwnerEffect);

	if (CurrentSwingEffectHandle.IsValid())
	{
		BP_RemoveSwingOwnerEffect();
	}
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	ensureCancelAbilityReturn(OwnerASC);
	CurrentSwingEffectHandle = UScWAbilitySystemGlobals::TryApplyGameplayEffectByClass(OwnerASC, WeaponActor->SwingOwnerEffect);
}

void UScWGameplayAbility_CommonSwing::BP_RemoveSwingOwnerEffect_Implementation()
{
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	ensureCancelAbilityReturn(OwnerASC);

	ensureCancelAbilityReturn(CurrentSwingEffectHandle.IsValid());
	OwnerASC->RemoveActiveGameplayEffect(CurrentSwingEffectHandle);

	CurrentSwingEffectHandle.Invalidate();
}
//~ End Swing
