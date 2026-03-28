// Scientific Ways

#include "Tags/ScWAbilitySystemExtensionsTags.h"

namespace FScWAbilitySystemExtensionsTags
{
	// Ability | Types
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Movement_Jump, "Ability.Type.Movement.Jump", "[Native] Jump movement ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Movement_Sprint, "Ability.Type.Movement.Sprint", "[Native] Sprint movement ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Movement_Crouch, "Ability.Type.Movement.Crouch", "[Native] Crouch movement ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Movement_WalkSlow, "Ability.Type.Movement.WalkSlow", "[Native] Slow walk movement ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Weapon_Swing, "Ability.Type.Weapon.Swing", "[Native] Melee weapon swing ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Interact, "Ability.Type.Interact", "[Native] Interaction ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Type_Flashlight, "Ability.Type.Flashlight", "[Native] Flashlight toggle ability");

	// State | Movement
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_Jumping, "Character.State.Jumping", "[Native] Character is currently jumping");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_Sprinting, "Character.State.Sprinting", "[Native] Character is currently sprinting");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_Crouching, "Character.State.Crouching", "[Native] Character is currently crouching");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_WalkingSlow, "Character.State.WalkingSlow", "[Native] Character is currently walking slowly");

	// State | Ability
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_FlashlightOn, "Character.State.FlashlightOn", "[Native] Character flashlight is enabled");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_Swinging, "Character.State.Swinging", "[Native] Melee swing state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_Aiming, "Character.State.Aiming", "[Native] Weapon aiming state");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Character_State_Interacting, "Character.State.Interacting", "[Native] Character is performing an interaction");
}
