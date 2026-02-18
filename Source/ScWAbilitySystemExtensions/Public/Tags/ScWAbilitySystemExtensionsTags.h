// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "NativeGameplayTags.h"

#define MODULE_API SCWABILITYSYSTEMEXTENSIONS_API

namespace FScWAbilitySystemExtensionsTags
{
	// Ability | Types
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Movement_Jump);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Movement_Sprint);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Movement_Crouch);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Movement_WalkSlow);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Weapon_Swing);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Interact);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Flashlight);

	// Pawn movement states
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Jumping);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Sprinting);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Crouching);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_WalkingSlow);

	// Pawn ability states
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_FlashlightOn);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Swinging);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Aiming);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Interacting);
}

#undef MODULE_API
