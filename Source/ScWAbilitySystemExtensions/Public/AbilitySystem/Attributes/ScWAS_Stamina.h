// Scientific Ways

#pragma once

#include "ScWAbilitySystemExtensions.h"

#include "ScWAS_Stamina.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Stamina Attributes"))
class UScWAS_Stamina : public UAttributeSet
{
	GENERATED_BODY()

public:

	UScWAS_Stamina();

//~ Begin Replication
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // UObject
//~ End Replication

//~ Begin Change
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& InAttribute, float& InOutNewValue) override; // UAttributeSet
	virtual void PostAttributeChange(const FGameplayAttribute& InAttribute, float InPrevValue, float InNewValue) override; // UAttributeSet
//~ End Change

//~ Begin Attributes
public:

	UPROPERTY(ReplicatedUsing = "OnRep_Stamina", BlueprintReadOnly)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(ThisClass, Stamina)

	UPROPERTY(ReplicatedUsing = "OnRep_StaminaMax", BlueprintReadOnly)
	FGameplayAttributeData StaminaMax;
	ATTRIBUTE_ACCESSORS(ThisClass, StaminaMax)
//~ End Attributes

//~ Begin Replication
protected:

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& InPrevValue);

	UFUNCTION()
	virtual void OnRep_StaminaMax(const FGameplayAttributeData& InPrevValue);
//~ End Replication
};
