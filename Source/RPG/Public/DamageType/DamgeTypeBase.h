// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "DamgeTypeBase.generated.h"

//What is the Damage Type of the object?
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	DT_Heal       UMETA(DisplayName = "Heal"), //Heal is considered a damage type, it just adds positive damage
	DT_Phys	      UMETA(DisplayName = "Physical"),
	DT_Fire 	  UMETA(DisplayName = "Fire"),
	DT_Ice        UMETA(DisplayName = "Ice"),
	DT_Elec       UMETA(DisplayName = "Elec"),
	DT_Light      UMETA(DisplayName = "Light"),
	DT_Dark       UMETA(DisplayName = "Dark"),
	DT_Arcane     UMETA(DisplayName = "Arcane"),
	DT_True       UMETA(DisplayName = "True")  //True damage means that it ignores armor. 
};

UCLASS()
class RPG_API UDamgeTypeBase : public UDamageType
{
	GENERATED_BODY()

public:

	UDamgeTypeBase();

	//What is the damage type of this weapon?
	UPROPERTY(EditDefaultsOnly, Category = "Damage Config")
		EDamageType DamageType;

};
