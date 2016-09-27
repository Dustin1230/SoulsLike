// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG.h"
#include "Public/Spells/BaseSpell.h"

// Sets default values
ABaseSpell::ABaseSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpellPSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Spell PS"));

}

void ABaseSpell::OnSpellUse_Implementation(class ABasePawn* User)
{
	
}


