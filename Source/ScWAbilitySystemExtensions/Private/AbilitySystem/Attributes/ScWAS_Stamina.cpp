// Scientific Ways

#include "AbilitySystem/Attributes/ScWAS_Stamina.h"

UScWAS_Stamina::UScWAS_Stamina()
{
	InitStamina(100.0f);
	InitStaminaMax(100.0f);
}

//~ Begin Replication
void UScWAS_Stamina::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // UObject
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Stamina);
	DOREPLIFETIME(ThisClass, StaminaMax);
}
//~ End Replication

//~ Begin Change
void UScWAS_Stamina::PreAttributeChange(const FGameplayAttribute& InAttribute, float& InOutNewValue) // UAttributeSet
{
	if (InAttribute == GetStaminaAttribute())
	{
		InOutNewValue = FMath::Clamp(InOutNewValue, 0.0f, GetStaminaMax());
	}
	else
	{
		Super::PreAttributeChange(InAttribute, InOutNewValue);
	}
}

void UScWAS_Stamina::PostAttributeChange(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue) // UAttributeSet
{
	Super::PostAttributeChange(InAttribute, InPrevValue, InNewValue);

	
}
//~ End Change

//~ Begin Replication
void UScWAS_Stamina::OnRep_Stamina(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Stamina, InPrevValue);
}

void UScWAS_Stamina::OnRep_StaminaMax(const FGameplayAttributeData& InPrevValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, StaminaMax, InPrevValue);
}
//~ End Replication