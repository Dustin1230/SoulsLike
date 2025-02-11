// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "Armor.generated.h"

//Armor slot: Where do we wear the armor?
UENUM(BlueprintType)
enum class EArmorSlotEnum : uint8
{
	AS_Head	      UMETA(DisplayName = "Head"),
	AS_Chest      UMETA(DisplayName = "Chest"),
	AS_Legs       UMETA(DisplayName = "Legs"),
	AS_Arms       UMETA(DisplayName = "Arms"),
	AS_Boots      UMETA(DisplayName = "Boots")
};

USTRUCT(BlueprintType)
struct FArmorInfo
{
	GENERATED_BODY()

	//What is the Armor Slot?
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EArmorSlotEnum Slot;

	//How high is the defense? Protects from Phys Damage types
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 ArmorRating;

	//How high is the Fire defense? Protects from Fire Damage types
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 FireRating;

	//How high is the Ice defense? Protects from Ice Damage types
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 IceRating;

	//How high is the Elec defense? Protects from Elec Damage types
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 ElecRating;

	//How high is the Light defense? Protects from Light Damage types
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 LightRating;

	//How high is the Dark defense? Protects from Dark Damage types
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 DarkRating;

	//How high is the Arcane defense? Protects from Arcane Damage types
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 ArcaneRating;

};

UCLASS()
class RPG_API AArmor : public AItem
{
	GENERATED_BODY()
	
public:

	AArmor();

	/* ArmorInfo, used to configure the armor*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Armor Info")
		FArmorInfo ArmorInfo;

	/*
	 * When the armor is Eqiuped. On Equip will add defense stats to the pawn
	 * @param OwningPawn: The pawn that is equiping the armor
	 */
	void OnEquip(class ABasePawn* OwningPawn);

	/*
	 * When the armor is DeEquiped On DeEquip will remove defense stats from the pawn
	 * @param OwningPawn: the pawn that is unEquiping the armor. 
	 */
	void OnDequip(ABasePawn* OwningPawn);

};
