// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/BasePawn.h"
#include "PlayerPawn.generated.h"

//Data table when it comes to level checks. Table is a CVS file.
USTRUCT(BlueprintType)
struct FLevelUpData : public FTableRowBase
{
	GENERATED_BODY()

	//The amount of XP needed to level up.
	UPROPERTY(EditDefaultsOnly)
		int32 XPNeeded;
};

//On level up, these stats are increase. Stored in a struct for easily fetchable variables on save and load.
USTRUCT(BlueprintType)
struct FLevelUpStats
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadWrite)
		int32 CurrentStatPoints;

	/*
	* Current level up points. On Level Up, these points determine if you want to level up Health or Mana. 
	* One point will increase the Max Health/Mana by 50.
	*/
	UPROPERTY(BlueprintReadWrite)
		int32 CurrentLevelUpPoints;
};

UCLASS()
class RPG_API APlayerPawn : public ABasePawn
{
	GENERATED_BODY()
	
public:

	APlayerPawn();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		virtual void AffectHealth(int32 Delta, TSubclassOf<class UDamgeTypeBase> DamageType) override;

	UFUNCTION(BlueprintCallable, Category = "Character Update")
		virtual void UpdateStats() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		bool GetIsSprinting() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		bool GetIsLockedOn() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void AffectXP(int32 Delta);
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
		bool CanLevelUp() const;

	UPROPERTY(VisibleDefaultsOnly, Category = Cape)
	class USkeletalMeshComponent* Cape;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Settings")
		float BaseForwardDodgeSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge Settings")
		float BaseBackStepSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Controller Settings")
		float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, Category = "Controller Settings")
		float BaseLookRightRate;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn Settings")
		float LockOnSearchRadius;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn Settings")
		float SideScanDistance;

	UPROPERTY(EditDefaultsOnly, Category = "LockOn Settings")
		float SideScanRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina CoolDown Settings")
		float NonEmptyStaminaCDSeconds;

	UPROPERTY(EditDefaultsOnly, Category = "Stamina CoolDown Settings")
		float EmptyStaminaCDSeconds;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings")
		FVector NormalCamPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings")
		FVector LockOnCamPosition;

	UPROPERTY(BlueprintReadOnly, Category = "Camera Settings")
		FVector CameraTargetPosition;

	UFUNCTION(BlueprintNativeEvent, Category = "Camera Events")
		void OnCameraLockOn();

	UFUNCTION(BlueprintNativeEvent, Category = "Camera Events")
		void OnCameraLockOff();

	//How fast do we Sprint? Stat is added with Agility.
	UPROPERTY(EditDefaultsOnly, Category = "Sprint Settings")
		float BaseSprintSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Level Up Info")
		UDataTable* LevelUpDataTable;

	UPROPERTY(BlueprintReadOnly, Category = "Level Up Info")
		FLevelUpStats LevelUpStats;

	UPROPERTY(EditDefaultsOnly, Category = "Level Up Info")
		UParticleSystem* LevelUpPS;

	UPROPERTY(EditDefaultsOnly, Category = "Level Up Info")
		USoundBase* LevelUpSound;

private: 

	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void LookRight(float Value);
	void LookUpRate(float Value);
	void LookRightRate(float Value);

	void SprintPress();
	void Sprint();

	void StopSprint();

	void DodgePress();

	void CalculateDodge();

	void DodgeRoll(FVector Direction);

	void CheckLevelUp();

	void ScanRight();
	void ScanLeft();
	
	void LookAtTarget();

	void LockOnPress();
	void SearchForTargets();

	void LockOn(ABasePawn* Target);
	void LockOff();
	void UpdateCameraLocation();
	void AddLockOnPitch(float Angle);
	void SideScanForTarget(float Direction);
	void ProcessLockOnScanHit(TArray<FHitResult> OutHits);

	/*
	*	HasLineOfSight(): A function that tests to make sure the player could "See" the pawn if they were to lokc onto them
	*	It does a linetrace starting at the players head, and ends at the test pawn looking for world objects, if it hits a
	*	world object, we can't "see" them. Used to prevent from the player locking on a target behind walls.
	*/
	bool HasLineOfSight(ABasePawn* TestPawn) const;
	float GetCameraAngleDelta() const;

	void AttackPress();

	bool bIsLockedOn;

	bool bIsSprinting;

	float GetDodgeStrength(float DodgeSpeed) const;

	float CurrentStaminaCDSeconds;
	FTimerHandle StaminaCDTimerHandle;
	void TickDownStaminaCD();
	bool ShouldRestoreStamina() const;

	/*
	 *Check for Direction Values, we run these pieces of code so that way we're not going at half speed when we do dodge, 
	 *Values that are greater than 0 will always result in 1 and values less than that will result in -1.
	*/
	float CorrectInputValue(float Value) const;

	bool CanDodge() const;

	bool CanSprint() const;

	bool IsMaxLevel() const;

	bool IsAttacking() const;
	bool CanDodgeCancel() const;
	bool CanAttackCancel() const;

	bool IsMoving(float ForwardValue, float RightValue) const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
		class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = Boom)
		class USpringArmComponent* CameraBoom;

};
