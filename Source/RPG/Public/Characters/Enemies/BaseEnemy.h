// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/BasePawn.h"
#include "BaseEnemy.generated.h"

UCLASS()
class RPG_API ABaseEnemy : public ABasePawn
{
	GENERATED_BODY()
	
public: 

	ABaseEnemy();

	/*
	* When there is a change in a Pawns Health. Declared virtual for extended functionality.
	* This is the Enemies version of AffectHealth, use this when calling this function
	* if incoming damage is meant for Enemy.
	*
	* @param Delta: The ammount of incoming "Damage"
	* @param IncDamageType: The type of damage coming in. Note: healing counts as possitive "Damage"
	*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		virtual void AffectHealth(int32 Delta, TSubclassOf<class UDamgeTypeBase> DamageType);

	/*When defeated, how much XP does the player gain?*/
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Info")
		int32 XPWorth;

	/*
	 * The possible "basic" attacks all enemies will have, and will be accessed through the enemies
	 * behavior tree, specialzed attacks should be for bosses only.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Enemy Info")
		TArray<UAnimMontage*> PossibleAttacks;

	UFUNCTION(BlueprintCallable, Category = "Character Update")
		virtual void UpdateStats() override;
};
