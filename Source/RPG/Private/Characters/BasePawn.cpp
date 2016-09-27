// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG.h"
#include "Public/Items/Item.h"
#include "Public/Items/Weapon.h"
#include "Public/Items/Armor.h"
#include "Public/Spells/BaseSpell.h"
#include "Public/DamageType/DamgeTypeBase.h"
#include "Public/Characters/BasePawn.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bIsDead = false;
	LevelInfo.Level = 1;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ABasePawn::BeginPlay()
{

	Super::BeginPlay();

	SpawnEquipment();
}

void ABasePawn::SpawnEquipment()
{

	//Checks to see if it is possible to spawn equipment first before spawning to prevent crashing.

	if (StartUpWeapon)
	{
		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(StartUpWeapon, GetActorLocation(), GetActorRotation());
		SetCurrentWeapon(SpawnedWeapon);
	}

	if (StartUpHead)
	{
		AArmor* SpawnedHead = GetWorld()->SpawnActor<AArmor>(StartUpHead, GetActorLocation(), GetActorRotation());
		EquipArmor(SpawnedHead);
	}

	if (StartUpChest)
	{
		AArmor* SpawnedChest = GetWorld()->SpawnActor<AArmor>(StartUpChest, GetActorLocation(), GetActorRotation());
		EquipArmor(SpawnedChest);
	}

	if (StartUpLegs)
	{
		AArmor* SpawnedLegs = GetWorld()->SpawnActor<AArmor>(StartUpLegs, GetActorLocation(), GetActorRotation());
		EquipArmor(SpawnedLegs);
	}

	if (StartUpBoots)
	{
		AArmor* SpawnedBoots = GetWorld()->SpawnActor<AArmor>(StartUpBoots, GetActorLocation(), GetActorRotation());
		EquipArmor(SpawnedBoots);
	}

	if (StartUpArms)
	{
		AArmor* SpawnedArms = GetWorld()->SpawnActor<AArmor>(StartUpArms, GetActorLocation(), GetActorRotation());
		EquipArmor(SpawnedArms);
	}
}

void ABasePawn::UpdateStats()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed + GetAgility();
}

void ABasePawn::AffectHealth(int32 Delta, TSubclassOf<UDamgeTypeBase> IncDamageType)
{

	UDamgeTypeBase* TypeOfDamage = Cast<UDamgeTypeBase>(IncDamageType->GetDefaultObject());

	if (TypeOfDamage)
	{
		if (TypeOfDamage->DamageType == EDamageType::DT_Heal)
		{
			Health += Delta;

			if (Health > GetMaxHealth())
				Health = GetMaxHealth();
		}

		else
		{

			int32 Defense = GetDefenseStat(TypeOfDamage);

			//If the resulting check would subtract to health instead of adding it, run the next line.
			//We multiply the delta by negative one so that way if an armor has negative defense instead of positive defense,
			//it the damage will increase instead of decrese.
			if ((Delta * -1) + Defense < 0)
			{
				Health += (Delta * -1) + Defense;
				CheckDead();
			}
		}
	}
}

int32 ABasePawn::GetDefenseStat(UDamgeTypeBase* TypeOfDamage) const
{
	switch (TypeOfDamage->DamageType)
	{
		default:
		case EDamageType::DT_Phys:
			return DefenseData.ArmorRating;
		
		case EDamageType::DT_Fire:
			return (DefenseData.ArmorRating / 2) + DefenseData.FireDefense;
			
		case EDamageType::DT_Elec:
			return (DefenseData.ArmorRating / 2) + DefenseData.ElecDefense;
	
		case EDamageType::DT_Ice:
			return (DefenseData.ArmorRating / 2) + DefenseData.IceDefense;
			
		case EDamageType::DT_Dark:
			return (DefenseData.ArmorRating / 2) + DefenseData.DarkDefense;

		case EDamageType::DT_Light:
			return (DefenseData.ArmorRating / 2) + DefenseData.LightDefense;
		
		case EDamageType::DT_Arcane:
			return (DefenseData.ArmorRating / 2) + DefenseData.ArcaneDefense;
		
		case EDamageType::DT_True:
			return 0;
	}

	//Failsafe
	return 0;
}

void ABasePawn::AffectStamina(float Delta)
{
	Stamina += Delta;

	if (Stamina > GetMaxStamina())
		Stamina = GetMaxStamina();

	if (Stamina < 0)
		Stamina = 0;
}

void ABasePawn::AffectMana(int32 Delta)
{
	Mana += Delta;

	if (Mana > GetMaxMana())
		Mana = GetMaxMana();

	if (Mana < 0)
		Mana = 0;
}

void ABasePawn::CheckDead()
{
	if (Health <= 0)
	{	
		Health = 0;
		bIsDead = true;
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		BeginCleanUp();
	}
}

void ABasePawn::BeginCleanUp()
{
	if (Equipment.CurrentWeapon)
	{
		Equipment.CurrentWeapon->Destroy();
		Equipment.CurrentWeapon->ConditionalBeginDestroy();
	}

	if (Equipment.HeadSlot)
	{
		Equipment.HeadSlot->Destroy();
		Equipment.HeadSlot->ConditionalBeginDestroy();
	}

	if (Equipment.ChestSlot)
	{
		Equipment.ChestSlot->Destroy();
		Equipment.ChestSlot->ConditionalBeginDestroy();
	}

	if (Equipment.ArmsSlot)
	{
		Equipment.ArmsSlot->Destroy();
		Equipment.ArmsSlot->ConditionalBeginDestroy();
	}

	if (Equipment.LegSlot)
	{
		Equipment.LegSlot->Destroy();
		Equipment.LegSlot->ConditionalBeginDestroy();
	}

	if (Equipment.BootsSlot)
	{
		Equipment.BootsSlot->Destroy();
		Equipment.BootsSlot->ConditionalBeginDestroy();
	}

}

void ABasePawn::EquipArmor(AArmor* NewArmor)
{
	switch (NewArmor->ArmorInfo.Slot)
	{
		default:
		case EArmorSlotEnum::AS_Head:
			ChangeHelmet(NewArmor);
			break;

		case EArmorSlotEnum::AS_Chest:
			ChangeChest(NewArmor);
			break;

		case EArmorSlotEnum::AS_Arms:
			ChangeArms(NewArmor);
			break;

		case EArmorSlotEnum::AS_Legs:
			ChangeLegs(NewArmor);
			break;

		case EArmorSlotEnum::AS_Boots:
			ChangeBoots(NewArmor);
			break;
	}
}

void ABasePawn::ChangeHelmet(AArmor* NewArmor)
{
	if (Equipment.HeadSlot)
		Equipment.HeadSlot->OnDequip(this);

	Equipment.HeadSlot = NewArmor;
	Equipment.HeadSlot->OnEquip(this);

}

void ABasePawn::ChangeChest(AArmor* NewArmor)
{
	if (Equipment.ChestSlot)
		Equipment.ChestSlot->OnDequip(this);

	Equipment.ChestSlot = NewArmor;
	Equipment.ChestSlot->OnEquip(this);

}

void ABasePawn::ChangeArms(AArmor* NewArmor)
{
	if (Equipment.ArmsSlot)
		Equipment.ArmsSlot->OnDequip(this);

	Equipment.ArmsSlot = NewArmor;
	Equipment.ArmsSlot->OnEquip(this);

}

void ABasePawn::ChangeLegs(AArmor* NewArmor)
{
	if (Equipment.LegSlot)
		Equipment.LegSlot->OnDequip(this);

	Equipment.LegSlot = NewArmor;
	Equipment.LegSlot->OnEquip(this);

}

void ABasePawn::ChangeBoots(AArmor* NewArmor)
{
	if (Equipment.BootsSlot)
		Equipment.BootsSlot->OnDequip(this);

	Equipment.BootsSlot = NewArmor;
	Equipment.BootsSlot->OnEquip(this);

}

void ABasePawn::DeltaArmor(int32 Delta)
{
	DefenseData.ArmorRating += Delta;
}

void ABasePawn::DeltaFireDefense(int32 Delta)
{
	DefenseData.FireDefense += Delta;
}

void ABasePawn::DeltaIceDefense(int32 Delta)
{
	DefenseData.IceDefense += Delta;
}

void ABasePawn::DeltaElecDefense(int32 Delta)
{
	DefenseData.ElecDefense += Delta;
}

void ABasePawn::DeltaArcaneDefense(int32 Delta)
{
	DefenseData.ArcaneDefense += Delta;
}

void ABasePawn::DeltaLightDefense(int32 Delta)
{
	DefenseData.LightDefense += Delta;
}

void ABasePawn::DeltaDarkDefense(int32 Delta)
{
	DefenseData.DarkDefense += Delta;
}

void ABasePawn::SetMaxHealth(int32 NewStat)
{
	Stats.MaxHealth = NewStat;
}

void ABasePawn::SetMaxStamina(float NewStat)
{
	Stats.MaxStamina = NewStat;
}

void ABasePawn::SetMaxMana(int32 NewStat)
{
	Stats.MaxMana = NewStat;
}

void ABasePawn::SetStrength(int32 NewStat)
{
	Stats.Strength = NewStat;
}

void ABasePawn::SetDexterity(int32 NewStat)
{
	Stats.Dexterity = NewStat;
}

void ABasePawn::SetAgility(int32 NewStat)
{
	Stats.Agility = NewStat;
}

void ABasePawn::SetMagic(int32 NewStat)
{
	Stats.Magic = NewStat;
}

void ABasePawn::SetLuck(int32 NewStat)
{
	Stats.Luck = NewStat;
}

void ABasePawn::SetIsDead(bool IsDead)
{
	bIsDead = IsDead;
}

void ABasePawn::SetCurrentTarget(ABasePawn* NewTarget)
{
	CurrentTarget = NewTarget;
}

void ABasePawn::SetCurrentWeapon(AWeapon* NewWeapon)
{
	if (Equipment.CurrentWeapon)
		Equipment.CurrentWeapon->OnDeEquip();

	Equipment.CurrentWeapon = NewWeapon;

	Equipment.CurrentWeapon->OnEquip(this);
}

int32 ABasePawn::GetMaxHealth() const
{
	return Stats.MaxHealth;
}

float ABasePawn::GetMaxStamina() const
{
	return Stats.MaxStamina;
}

int32 ABasePawn::GetMaxMana() const
{
	return Stats.MaxMana;
}

int32 ABasePawn::GetStrength() const
{
	return Stats.Strength;
}

int32 ABasePawn::GetDexterity() const
{
	return Stats.Dexterity;
}

int32 ABasePawn::GetAgility() const
{
	return Stats.Agility;
}

int32 ABasePawn::GetMagic() const
{
	return Stats.Magic;
}

int32 ABasePawn::GetLuck() const
{
	return Stats.Luck;
}

bool ABasePawn::GetIsDead() const
{
	return bIsDead;
}

FEquipmentStruct ABasePawn::GetEquipment() const
{
	return Equipment;
}

FLevelInfo ABasePawn::GetLevelInfo() const
{
	return LevelInfo;
}

FInventoryItemStruct ABasePawn::GetInventoryItem(int32 Index) const
{
	//Dev note, we run a check BEFORE this function is called to make sure the index is valid.
	return Inventory.Items[Index];
}

FInventoryStruct ABasePawn::GetInventory() const
{
	//Dev note, we run a check BEFORE this function is called to make sure the index is valid.
	return Inventory;
}

FStatsStruct ABasePawn::GetStats() const
{
	//Dev note, we run a check BEFORE this function is called to make sure the index is valid.
	return Stats;
}

int32 ABasePawn::GetGold() const
{
	return Inventory.Gold;
}

ABasePawn* ABasePawn::GetCurrentTarget() const
{
	return CurrentTarget;
}

AWeapon* ABasePawn::GetCurrentWeapon() const
{
	return Equipment.CurrentWeapon;
}
