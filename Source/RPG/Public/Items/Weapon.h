// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "Weapon.generated.h"

//For animation handling purposes.
UENUM(BlueprintType)		
enum class EWeaponTypeEnum : uint8
{
	WT_OneHand	  UMETA(DisplayName = "1Hand"),
	WT_TwoHand 	  UMETA(DisplayName = "2Hand"),
	WT_Staff      UMETA(DisplayName = "Staff"),
	WT_Bow        UMETA(DisplayName = "Bow"),
	WT_CrossBow   UMETA(DisplayName = "CrossBow"),
};

//What Stat governs the damage of this weapon?
UENUM(BlueprintType)
enum class EWeaponStatEnum : uint8
{
	WS_Strength	  UMETA(DisplayName = "Strength"),
	WS_Dexterity  UMETA(DisplayName = "Dexterity"),
	WS_Magic      UMETA(DisplayName = "Magic"),
	WS_Luck       UMETA(DisplayName = "Luck")
};

//What isd the current status of the weapon?
UENUM(BlueprintType)
enum class  EWeaponStatus : uint8
{
	WS_Idle	      UMETA(DisplayName = "Idle"),
	WS_Startup    UMETA(DisplayName = "Startup"),
	WS_Active     UMETA(DisplayName = "Active"),
	WS_Recovery   UMETA(DisplayName = "Recovery")
};

USTRUCT(BlueprintType)
struct FPlayerAttackData
{
	GENERATED_BODY()

	/*What is the attack animation for the player*/
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* AttackAnimation;

	/*The animations playrate*/
	UPROPERTY(EditDefaultsOnly)
		float PlayRate;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	/*How much damage does this weapon do?*/
	UPROPERTY(EditDefaultsOnly)
		int32 Damage;

	/*How much required mana does this weapon use? 
	*
	* Note: Should only be exclusive to staves/magic based weapons
	*/
	UPROPERTY(EditDefaultsOnly)
		int32 RequiredMana;

	/*What is the chance this weapon will crit?
	* 0 = No chance.
	* 100 = Always.
	*/
	UPROPERTY(EditDefaultsOnly)
		float CritChance;

	/*When the weapon crits, what is the bonus damage?*/
	UPROPERTY(EditDefaultsOnly)
		float CritBonus;

	/*When equiping the weapon, what slot does the weapon attach to?*/
	UPROPERTY(EditDefaultsOnly)
		FName WeaponAttachPoint;

};

UCLASS()
class RPG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	/*Weapon Data, used to configure weapon*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
		FWeaponData WeaponData;

	/*
	* When the weapon is equiped
	* @param NewOwner: the new owning pawn of the weapon.
	*/
	void OnEquip(class ABasePawn* NewOwner);

	/*When the weapon is DeEquiped.*/
	void OnDeEquip();

	/*
	 * Set the current owning pawn of the weapon.
	 * @param NewOwner: the new owning pawn of this weapon
	 */
	UFUNCTION(BlueprintCallable, Category = "Weaapon Setters")
		void SetOwningPawn(ABasePawn* NewOwner);

	/*
	 * Sets the current status of the weapon.
	 * @param NewStatus: The new status of the weapon, status such as Idle, Active, etc.
	 */
	UFUNCTION(BlueprintCallable, Category = "Weaapon Setters")
		void SetCurrentStatus(EWeaponStatus NewStatus);

	/*Returns the owning pawn of the weapon*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weaapon Getters")
		class ABasePawn* GetOwningPawn() const;

	/*Returns the weapons type*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weaapon Getters")
		EWeaponTypeEnum GetWeaponType() const;
	
	/*Returns the current status of the weapon*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weaapon Getters")
		EWeaponStatus GetCurrentStatus() const;

	/*
	* Player attack data. The size should be two, any more and the player will ignore other attacks beyond index 1
	* 0 = 1st attack and 1 = followup, and will keep alternating between two attacks.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Player Attack Data")
		TArray<FPlayerAttackData> PlayerAttackData;

protected:

	/*What is the weapons type? Is used for animation handling purposes. A different type will alter the players animation state*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
		EWeaponTypeEnum WeaponType;
	
	/*What is the weapons determining stat? Used for calculating bonus damage*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
		EWeaponStatEnum WeaponStat;

	/*The current status of the weapon. Anything that isn't "active" cannot damage*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
		EWeaponStatus CurrentStatus;

	/*
	* What is the damage type? It is passed when the weapon does damage.
	* Read the UDamgeTypeBase class for more info on damage types.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere = "Weapon Config")
		TSubclassOf<class UDamgeTypeBase> DamageType;

	/*The weapon mesh*/
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* WeaponSkelMesh;

	/*Weapons Hitbox, should not necessesirly be accurate.*/
	UPROPERTY(VisibleDefaultsOnly, Category = Hitbox)
		class UBoxComponent* Hitbox;

	/*Hit Sound.*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
		USoundBase* HitSound;

	/*The owning pawn of the weapon.*/
	class ABasePawn* OwningPawn;

	/*When we do hit something, returns final damage.*/
	UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
		int32 DamageEvent() const;

	/*Checks to see if the weapon is currently "active"*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weaapon Getters")
		bool CanDamage() const;

private:

	/*Return whether or not the weapon was a crit*/
	bool WasCrit() const;

	/*Returns the weapons damage + stat modifier, used in calculation for final damage*/
	int32 GetDamage() const;
	
	/*Returns the owning pawns stat multiplier. IE, a strength based weapon will returns the owning pawns strength.*/
	int32 GetStatMultiplier() const;
	
};
