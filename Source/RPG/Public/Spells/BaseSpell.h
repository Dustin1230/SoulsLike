// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseSpell.generated.h"


USTRUCT(BlueprintType)
struct FSpellInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		FName SpellName;

	UPROPERTY(EditDefaultsOnly)
		FString SpellDescription;

	UPROPERTY(EditDefaultsOnly)
		int32 ManaCost;

	//Effectiveness can mean anything. From Damage to health.
	UPROPERTY(EditDefaultsOnly)
		int32 SpellEffectiveness;

	UPROPERTY(EditDefaultsOnly)
		UTexture2D* SpellImage;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* SpellAnim;

};

UCLASS()
class RPG_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ABaseSpell();

	UPROPERTY(EditDefaultsOnly, Category = "Spell Info")
		FSpellInfo SpellInfo;

	UFUNCTION(BlueprintNativeEvent, Category = "Spell Functions")
		void OnSpellUse(class ABasePawn* User);


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Spell Info")
		TSubclassOf<class UDamgeTypeBase> DamageType;

	UPROPERTY(VisibleDefaultsOnly, Category = PSC)
		UParticleSystemComponent* SpellPSC;

};
