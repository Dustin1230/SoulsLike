// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"

//Item Description: When this Item is in our inventory, what is the Items "Info"
USTRUCT(BlueprintType)
struct FItemDescriptStruct
{
	GENERATED_BODY()

	/*
	* The name of the Item.
	*/
	UPROPERTY(EditDefaultsOnly)
		FName ItemName;

	/*
	* The items description.
	*/
	UPROPERTY(EditDefaultsOnly)
		FString ItemDescription;

	/*
	* Image of the Item
	*/
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* ItemImage;

};

UCLASS()
class RPG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	// Sets default values for this actor's properties
	AItem();

protected:

	//Item description, Name, Description, etc.
	UPROPERTY(EditDefaultsOnly, Category = "ItemDescription")
		FItemDescriptStruct ItemDescription;

};
