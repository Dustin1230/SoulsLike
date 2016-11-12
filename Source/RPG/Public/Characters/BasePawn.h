// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BasePawn.generated.h"

USTRUCT(BlueprintType)
struct FLevelInfo
{
	GENERATED_BODY()

   /*
	* The current level of the Pawn.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 Level;
	
   /*
	* The current XP of the Pawn.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		int32 XP;
};

USTRUCT(BlueprintType)
struct FDefenseData
{
	GENERATED_BODY()

	/*The Armor rating of the pawn: blocks all incoming physical damage*/
	UPROPERTY(BlueprintReadOnly)
		int32 ArmorRating;

	/*The Fire defense of the pawn: blocks all incoming Fire damage*/
	UPROPERTY(BlueprintReadOnly)
		int32 FireDefense;

	/*The Ice defense of the pawn: blocks all incoming Ice damage*/
	UPROPERTY(BlueprintReadOnly)
		int32 IceDefense;

	/*The Elec defense of the pawn: blocks all incoming Elec damage*/
	UPROPERTY(BlueprintReadOnly)
		int32 ElecDefense;

	/*The Light defense of the pawn: blocks all incoming Light damage*/
	UPROPERTY(BlueprintReadOnly)
		int32 LightDefense;

	/*The Dark defense of the pawn: blocks all incoming Dark damage*/
	UPROPERTY(BlueprintReadOnly)
		int32 DarkDefense;

	/*The Arcane defense of the pawn: blocks all incoming Arcane damage*/
	UPROPERTY(BlueprintReadOnly)
		int32 ArcaneDefense;
};

USTRUCT(BlueprintType)
struct FStatsStruct
{
	GENERATED_BODY()
		
	/*
	 * Determines Pawns maximum Health.
	 */
	UPROPERTY(EditDefaultsOnly)
		int32 MaxHealth;

	/*
	* Determines Pawns maximum Health.
	*/
	UPROPERTY(EditDefaultsOnly)
		int32 MaxStamina;

   /*
	* Determines Pawns maximum Mana.
	*/
	UPROPERTY(EditDefaultsOnly)
		int32 MaxMana;

	/*
	 * Determines how strong you are, higher strength values allow you to wield 
	 * strength based weapons like Two Handed Weapons, and deal more damage to
	 * weapons that are exclusive to strength.
	 */
	UPROPERTY(EditDefaultsOnly)
		int32 Strength;

   /*
	* Determines your finese with Dex based weapons, such as bows or daggers.
	* Higher Dex Values means you can strike more quickly with all weapon types 
	* and deal more damage with Dex based weapons
	*/
	UPROPERTY(EditDefaultsOnly)
		int32 Dexterity;

   /*
	* Determines how fast you are, higher Agility values means you will move quicker
	* and dodge further.
	*/
	UPROPERTY(EditDefaultsOnly)
		int32 Agility;

	/*
	 * Determines how good your skills are with magic, higher magic stats will allow you 
	 * to use more powerful spells and deal more damage with harmful spells.
	 */
	UPROPERTY(EditDefaultsOnly)
		int32 Magic;


   /*
	* Determines how lucky you are, effects a little bit of everything.
    */
	UPROPERTY(EditDefaultsOnly)
		int32 Luck;

	FStatsStruct()
	{	
		Strength = 0;
		Dexterity = 0;
		Agility = 0;
		Magic = 0;
		Luck = 0;		
	}

};

USTRUCT(BlueprintType)
struct FEquipmentStruct
{
	
	GENERATED_BODY()

   /*
	* The Pawns CurrentWeapon.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class AWeapon* CurrentWeapon;

   /*
	* The Pawns Head Armor Slot.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class AArmor* HeadSlot;

   /*
	* The Pawns Chest Armor Slot.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class AArmor* ChestSlot;
	
	/*
	* The Pawns Leg Armor Slot.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class AArmor* LegSlot;

   /*
	* The Pawns Arms Armor Slot.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class AArmor* ArmsSlot;

   /*
	* The Pawns Chest Boots Slot.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class AArmor* BootsSlot;
};


USTRUCT(BlueprintType)
struct FInventoryItemStruct
{

	GENERATED_BODY()

   /*
	* The inventory item. Declared as a subclass so that way 
	* if the game has a saving feature, it will can easily save the item. 
	* And when we come back, the item will be there on load. 
	*/
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AItem> Item;

   /*
	* The count of the item. Since our FInventoryItemStruct will be an array, if there 
	* is a duplicate of the same item (also another reason we chose SubClass rather than pointer) in the array,
	* it will increase that "count" of said item at that array index.
	*/
	UPROPERTY(EditDefaultsOnly)
		int32 Count;

};

USTRUCT(BlueprintType)
struct FInventoryStruct
{

	GENERATED_BODY()

   /*
	* The Pawns inventory items.
	*/
	UPROPERTY(EditDefaultsOnly)
		TArray<FInventoryItemStruct> Items;

   /*
	* The amount of gold on the Pawn. Gold is this games Currency.  
	*/
	UPROPERTY(EditDefaultsOnly)
		int32 Gold;

};

UCLASS()
class RPG_API ABasePawn : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	ABasePawn();


	virtual void BeginPlay() override;

   /*
	* When there is a change in a Pawns Health. Declared virtual for extended functionality. 
	* Children of this class will call the this function, and then do other checks as needed.
	* @param Delta: The ammount of incoming "Damage"
	* @param IncDamageType: The type of damage coming in. Note: healing counts as possitive "Damage"
	*/
	virtual void AffectHealth(int32 Delta, TSubclassOf<class UDamgeTypeBase> IncDamageType);

	/*
	* Gets the defense stat so that way incoming damage can be reduced base on the damage type
	* @param TypeOfDamage: The incoming damage type.
	*/
	int32 GetDefenseStat(UDamgeTypeBase* TypeOfDamage) const; 
	
	/*Check Dead: Checks to see if the pawn is at 0 health*/
	void CheckDead();
	
	/*BeginCleanUp(): Start to delete actors that are no longer needed, like armor and weapons*/
	void BeginCleanUp();

	/*
	* When there is a change in the Pawns Stamina.
	* @param Delta: The amount changed in the Stamina
	*/
	void AffectStamina(float Delta);

   /*
	* When there is a change in the Pawns Mana.
	* @param Delta: The amount changed in the Mana
	*/
	void AffectMana(int32 Delta);

	/*
	* Gets the Max Health of the Pawn.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		int32 GetMaxHealth() const;

	/*
	* Gets the Max Stamina of the Pawn.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		float GetMaxStamina() const;

	/*
	* Gets the Max Mana of the Pawn.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		int32 GetMaxMana() const;

	/*
	* Gets the Strength of the Pawn.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		int32 GetStrength() const;

	/*
	* Gets the Dexterity of the Pawn.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		int32 GetDexterity() const;

	/*
	* Gets the Agility of the Pawn
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		int32 GetAgility() const;

	/*
	* Gets the Magic of the Pawn
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		int32 GetMagic() const;

	/*
	* Gets the Luck of the Pawn
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		int32 GetLuck() const;

	/*
	* Gets the bIsDead variable.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		bool GetIsDead() const;

   /*
	* Gets the Pawns Equipment
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		FEquipmentStruct GetEquipment() const;

	/*
	* Gets the LevelInfo of the Pawn.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		FLevelInfo GetLevelInfo() const;

	/*
	* Gets the Pawns invetory item at said index.
	* 
	* Note: Before calling this function, use TArray's "IsValidIndex(Index)" function
	*       to check to see if the array item exists, because this function doesn't
	*       check it for you. 
	*
 	* @param Index: The index you are trying to access
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		FInventoryItemStruct GetInventoryItem(int32 Index) const;

	/*Returns the pawns inventory*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		FInventoryStruct GetInventory() const;

	/*Returns the pawns stats.*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		FStatsStruct GetStats() const;

	/*
	* Gets the Pawns current Gold.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		int32 GetGold() const;

	/*
	* Gets the Pawns Current Target.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		ABasePawn* GetCurrentTarget() const;

	/*
	* Gets the Pawns Current Weapon.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		class AWeapon* GetCurrentWeapon() const;

	/*
	* Sets the Max health of the Pawn.
	* @param NewStat: The new stat for this specfic stat.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetMaxHealth(int32 NewStat);

	/*
	* Sets the Max health of the Pawn.
	* @param NewStat: The new stat for this specfic stat.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetMaxStamina(float NewStat);

	/*
	* Set the Max mana of the Pawn.
	* @param NewStat: The new stat for this specfic stat.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetMaxMana(int32 NewStat);

	/*
	* Set the Strength of the Pawn.
	* @param NewStat: The new stat for this specfic stat.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetStrength(int32 NewStat);

	/*
	* Set the Dexterity of the Pawn.
	* @param NewStat: The new stat for this specfic stat.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetDexterity(int32 NewStat);

	/*
	* Set the Agility of the Pawn.
	* @param NewStat: The new stat for this specfic stat.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetAgility(int32 NewStat);

	/*
	* Set the Magic of the Pawn.
	* @param NewStat: The new stat for this specfic stat.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetMagic(int32 NewStat);

	/*
	* Set the Luck of the Pawn.
	* @param NewStat: The new stat for this specfic stat.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetLuck(int32 NewStat);

	/*
	* Set the bIsDead variable
	* @param isDead: Is the pawn dead?
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetIsDead(bool IsDead);

	/*
	* Set the Current Target of the Pawn.
	* @param NewTarget: The new target the pawn will look at.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetCurrentTarget(ABasePawn* NewTarget);

	/*
	* Set the Current Weapon of the Pawn.
	* @param NewWeapon: The new weapon the pawn will equip.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void SetCurrentWeapon(AWeapon* NewWeapon);

	/*
	* Equip a new piece of armor.
	* @param NewArmor: The new piece of armor that is to be equiped.
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void EquipArmor(AArmor* NewArmor);
	
		void ChangeHelmet(AArmor* NewArmor);
		void ChangeChest(AArmor* NewArmor);
		void ChangeArms(AArmor* NewArmor);
		void ChangeLegs(AArmor* NewArmor);
		void ChangeBoots(AArmor* NewArmor);

	/*
	* A change in the armor of the pawn
	* @param Delta: How much armor is being removed or added?
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void DeltaArmor(int32 Delta);

	/*
	* A change in the fire defense of the pawn
	* @param Delta: How much fire defense is being removed or added?
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void DeltaFireDefense(int32 Delta);

	/*
	* A change in the Ice defense of the pawn
	* @param Delta: How much ice defense is being removed or added?
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void DeltaIceDefense(int32 Delta);

	/*
	* A change in the electric defense of the pawn
	* @param Delta: How much electric defense is being removed or added?
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void DeltaElecDefense(int32 Delta);

	/*
	* A change in the Arcane defense of the pawn
	* @param Delta: How much Arcane defense is being removed or added?
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void DeltaArcaneDefense(int32 Delta);

	/*
	* A change in the light defense of the pawn
	* @param Delta: How much light defense defense is being removed or added?
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void DeltaLightDefense(int32 Delta);

	/*
	* A change in the dark defense of the pawn
	* @param Delta: How much dark defense is being removed or added?
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn Setters")
		void DeltaDarkDefense(int32 Delta);

protected:

	/*The pawns defesne data, such as armor, and elemental resistences*/
	UPROPERTY(BlueprintReadOnly, Category = "Defense Data")
		FDefenseData DefenseData;

	/*
	* The Pawns Current health.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pawn Variables")
		int32 Health;

	/*
	* The Pawns Current health.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pawn Variables")
		float Stamina;

	/*
	* The Pawns Current Mana
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pawn Variables")
		int32 Mana;

	/*
	*  The Pawns Base walk speed. This stat is added with Agility to determine final
	*  walk speed.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pawn Variables")
		float BaseWalkSpeed;

	/*
	* Is the Pawn Dead?
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Pawn Variables")
		bool bIsDead;

	/*
	*  The Pawns Stats.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		FStatsStruct Stats;

	/*
	* The Pawns Equipment.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		FEquipmentStruct Equipment;

	/*
	* The Pawns Inventory.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		FInventoryStruct Inventory;

	/*
	* The Pawns Level Info.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Level Info")
		FLevelInfo LevelInfo;

	/*Startup Equipment*/
	UPROPERTY(EditDefaultsOnly, Category = "Start Up")
		TSubclassOf<AWeapon> StartUpWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Start Up")
		TSubclassOf<AArmor> StartUpHead;

	UPROPERTY(EditDefaultsOnly, Category = "Start Up")
		TSubclassOf<AArmor> StartUpChest;

	UPROPERTY(EditDefaultsOnly, Category = "Start Up")
		TSubclassOf<AArmor> StartUpArms;

	UPROPERTY(EditDefaultsOnly, Category = "Start Up")
		TSubclassOf<AArmor> StartUpLegs;

	UPROPERTY(EditDefaultsOnly, Category = "Start Up")
		TSubclassOf<AArmor> StartUpBoots;

	/*
	* The Pawns Current Target. Not a UPROPERTY since it is being set and get in Blueprint, and doesnt need to be
	* editted in the edit tab. 
	*/
	ABasePawn* CurrentTarget;

	/*
	* UpdateStats. A function that Updates certain variables that need to be updated whenever our stats change.
	* Declared virtual for extended functionality. So that way when children of this class call their UpdateStats, it'll
	* not only update all BasePawn stats, but also stats that are exclusive to their class. 
	*/
	virtual void UpdateStats();


	/*On begin play, all the pawns equipment will be spawned.*/
	void SpawnEquipment();

};
