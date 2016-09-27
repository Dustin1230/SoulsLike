// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG.h"
#include "Public/Characters/Player/PlayerPawn.h"
#include "Public/Characters/Enemies/BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{

}

void ABaseEnemy::UpdateStats()
{
	Super::UpdateStats();
}

void ABaseEnemy::AffectHealth(int32 Delta, TSubclassOf<class UDamgeTypeBase> DamageType)
{
	//IF we received damage but are dead, don't run this function
	if (!bIsDead)
	{
		Super::AffectHealth(Delta, DamageType);

		if (bIsDead)
		{
			DetachFromControllerPendingDestroy();

			auto PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

			if (PlayerPawn)
				PlayerPawn->AffectXP(XPWorth);

			SetLifeSpan(3.f);
		}
	}
}


