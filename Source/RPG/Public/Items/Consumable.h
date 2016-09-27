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

	UFUNCTION(BlueprintNativeEvent, Category = "Consumable Functions")
		void OnConsume(class ABasePawn* Comsumer);
	
	
	
};
