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

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		virtual void AffectHealth(int32 Delta, TSubclassOf<class UDamgeTypeBase> DamageType);

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Info")
		int32 XPWorth;


	UFUNCTION(BlueprintCallable, Category = "Character Update")
		virtual void UpdateStats() override;
};
