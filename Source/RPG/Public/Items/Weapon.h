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

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* AttackAnimation;

	UPROPERTY(EditDefaultsOnly)
		float PlayRate;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		int32 Damage;

	UPROPERTY(EditDefaultsOnly)
		int32 RequiredMana;

	UPROPERTY(EditDefaultsOnly)
		float CritChance;

	UPROPERTY(EditDefaultsOnly)
		float CritBonus;

	UPROPERTY(EditDefaultsOnly)
		FName WeaponAttachPoint;

};

UCLASS()
class RPG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
		FWeaponData WeaponData;

	void OnEquip(class ABasePawn* NewOwner);

	void OnDeEquip();

	UFUNCTION(BlueprintCallable, Category = "Weaapon Setters")
		void SetOwningPawn(ABasePawn* NewOwner);

	UFUNCTION(BlueprintCallable, Category = "Weaapon Setters")
		void SetCurrentStatus(EWeaponStatus NewStatus);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weaapon Getters")
		class ABasePawn* GetOwningPawn() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weaapon Getters")
		EWeaponTypeEnum GetWeaponType() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weaapon Getters")
		EWeaponStatus GetCurrentStatus() const;

	UPROPERTY(EditDefaultsOnly, Category = "Player Attack Data")
		TArray<FPlayerAttackData> PlayerAttackData;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
		EWeaponTypeEnum WeaponType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
		EWeaponStatEnum WeaponStat;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Config")
		EWeaponStatus CurrentStatus;

	UPROPERTY(BlueprintReadOnly, EditAnywhere = "Weapon Config")
		TSubclassOf<class UDamgeTypeBase> DamageType;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* WeaponSkelMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Hitbox)
		class UBoxComponent* Hitbox;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
		USoundBase* HitSound;

	class ABasePawn* OwningPawn;

	UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
		int32 DamageEvent() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weaapon Getters")
		bool CanDamage() const;

private:

	bool WasCrit() const;

	int32 GetDamage() const;
	
	int32 GetStatMultiplier() const;
	
};
