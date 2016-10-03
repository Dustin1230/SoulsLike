// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG.h"
#include "Public/Characters/BasePawn.h"
#include "Public/Items/Weapon.h"

AWeapon::AWeapon()
{
	WeaponSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkelMesh"));
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));

	RootComponent = WeaponSkelMesh;

	//Setup attachment to root component so it will move with the sword mesh.
	Hitbox->SetupAttachment(RootComponent);

}

void AWeapon::OnEquip(ABasePawn* NewOwner)
{
	SetOwningPawn(NewOwner);

	AttachToComponent(OwningPawn->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponData.WeaponAttachPoint);
}

void AWeapon::OnDeEquip()
{
	Destroy(); //Clean up memory
	ConditionalBeginDestroy();
}

void AWeapon::SetOwningPawn(ABasePawn* NewOwner)
{
	OwningPawn = NewOwner;
}

void AWeapon::SetCurrentStatus(EWeaponStatus NewStatus)
{
	CurrentStatus = NewStatus;
}

int32 AWeapon::DamageEvent() const
{
	//We hit something, play a sound
	if(HitSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());

	int32 FinalDamage = WasCrit() ? GetDamage() * WeaponData.CritBonus : GetDamage();
	
	return FinalDamage;
}

int32 AWeapon::GetDamage() const
{
	return WeaponData.Damage + GetStatMultiplier();
}

int32 AWeapon::GetStatMultiplier() const
{
	//If we have an owning pawn. 
	if (OwningPawn)
	{
		switch (WeaponStat)
		{
			case EWeaponStatEnum::WS_Strength:
				return GetOwningPawn()->GetStrength();

			case EWeaponStatEnum::WS_Dexterity:
				return GetOwningPawn()->GetDexterity();

			case EWeaponStatEnum::WS_Magic:
				return GetOwningPawn()->GetMagic();

			case EWeaponStatEnum::WS_Luck:
				return GetOwningPawn()->GetLuck();
		}
	}

	//Failsafe
	return 1;
}

bool AWeapon::WasCrit() const
{
	//Luck effects chances of getting a crit
	return (FMath::RandRange(0, 100) - (GetOwningPawn()->GetLuck() / 10)) < WeaponData.CritChance;
}

bool AWeapon::CanDamage() const
{
	return CurrentStatus == EWeaponStatus::WS_Active;
}

ABasePawn* AWeapon::GetOwningPawn() const
{
	return OwningPawn;
}

EWeaponTypeEnum AWeapon::GetWeaponType() const
{
	return WeaponType;
}

EWeaponStatus AWeapon::GetCurrentStatus() const
{
	return CurrentStatus;
}


