// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "Consumable.generated.h"

UCLASS()
class RPG_API AConsumable : public AItem
{
	GENERATED_BODY()

public:

	AConsumable();

	/*
	* What happens when the pawn consumes the consumable?
	* @param Consumer: The pawn who is "Consuming" the item
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Consumable Functions")
		void OnConsume(class ABasePawn* Comsumer);
	
	
	
};
