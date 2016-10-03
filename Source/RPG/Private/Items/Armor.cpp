// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG.h"
#include "Public/Items/Armor.h"
#include "Public/Characters/BasePawn.h"

AArmor::AArmor()
{
	
}

void AArmor::OnEquip(ABasePawn* OwningPawn)
{
	OwningPawn->DeltaArmor(ArmorInfo.ArmorRating);
	OwningPawn->DeltaArcaneDefense(ArmorInfo.ArcaneRating);
	OwningPawn->DeltaFireDefense(ArmorInfo.FireRating);
	OwningPawn->DeltaElecDefense(ArmorInfo.ElecRating);
	OwningPawn->DeltaIceDefense(ArmorInfo.IceRating);
	OwningPawn->DeltaDarkDefense(ArmorInfo.DarkRating);
	OwningPawn->DeltaLightDefense(ArmorInfo.LightRating);
}

void AArmor::OnDequip(ABasePawn* OwningPawn)
{

	OwningPawn->DeltaArmor(ArmorInfo.ArmorRating * -1);
	OwningPawn->DeltaArcaneDefense(ArmorInfo.ArcaneRating * -1);
	OwningPawn->DeltaFireDefense(ArmorInfo.FireRating * -1);
	OwningPawn->DeltaElecDefense(ArmorInfo.ElecRating * -1);
	OwningPawn->DeltaIceDefense(ArmorInfo.IceRating * -1);
	OwningPawn->DeltaDarkDefense(ArmorInfo.DarkRating * -1);
	OwningPawn->DeltaLightDefense(ArmorInfo.LightRating * -1);

	/*
		Since it's not active anymore, destroy the actor to save up memory.
		And since there is a reference to the unequiped armors class in the owning pawns inventory,
		we don't need to re-add it to the inventory.
	*/

	Destroy();
	ConditionalBeginDestroy();
}
