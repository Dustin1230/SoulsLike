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

	/* Current Stat points, points that go into a characters stats, like strength, agility, etc.*/
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

	//Constructor
	APlayerPawn();

	/*When the pawn can recieve input*/
	virtual void BeginPlay() override;

	/*
	* When there is a change in a Pawns Health. Declared virtual for extended functionality.
	* This is the players version of AffectHealth, use this when calling this function 
	* if incoming damage is meant for player.
	*
	* @param Delta: The ammount of incoming "Damage"
	* @param IncDamageType: The type of damage coming in. Note: healing counts as possitive "Damage"
	*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		virtual void AffectHealth(int32 Delta, TSubclassOf<class UDamgeTypeBase> DamageType) override;

	UFUNCTION(BlueprintCallable, Category = "Character Update")
		virtual void UpdateStats() override;

	/*Checks to see if the pawn is sprinting*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		bool GetIsSprinting() const;

	/*Checks to see if the pawn is locked on*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Pawn Getters")
		bool GetIsLockedOn() const;

	/* 
	*	A Change in the pawns XP
	*   @param Delta: The ammount of XP that is getting added.
	*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void AffectXP(int32 Delta);
	
	/*Tick, called every frame*/
	virtual void Tick(float DeltaSeconds) override;
	
protected:

	/*Checks to see if the pawn can level up, checks to see if it has level up points.*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
		bool CanLevelUp() const;

	/*Pawn cape, nothing special, just looks cool.*/
	UPROPERTY(VisibleDefaultsOnly, Category = Cape)
	class USkeletalMeshComponent* Cape;

	/*
	 * When moving (regardless if moving forward or right), how far do we go?
	 * Higher values means further out.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Dodge Settings")
		float BaseForwardDodgeSpeed;

	/*When backsteping, how far do we go? Higher values means further out.*/
	UPROPERTY(EditDefaultsOnly, Category = "Dodge Settings")
		float BaseBackStepSpeed;

	/*
	 * When using the controller, what is the input rate at which it goes up/down?
	 * Higher values means faster camera movements.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Controller Settings")
		float BaseLookUpRate;

	/*
	* When using the controller, what is the input rate at which it goes left/right?
	* Higher values means faster camera movements.
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Controller Settings")
		float BaseLookRightRate;

	/*In centermeters, what is the radius of the lock on search?*/
	UPROPERTY(EditDefaultsOnly, Category = "LockOn Settings")
		float LockOnSearchRadius;

	/*When we do a side scan, in centimeters, how far does the trace search go out?*/
	UPROPERTY(EditDefaultsOnly, Category = "LockOn Settings")
		float SideScanDistance;

	/*When we do a side scane, in centimeters, what is the radius of the search?*/
	UPROPERTY(EditDefaultsOnly, Category = "LockOn Settings")
		float SideScanRadius;

	/*In seconds, how many seconds does it take to start restoring stamina if we DIDN'T run out of stamina?*/
	UPROPERTY(EditDefaultsOnly, Category = "Stamina CoolDown Settings")
		float NonEmptyStaminaCDSeconds;

	/*In seconds, how many seconds does it take to start restoring stamina if we DID run out of stamina?*/
	UPROPERTY(EditDefaultsOnly, Category = "Stamina CoolDown Settings")
		float EmptyStaminaCDSeconds;

	/*When not locked on, where is the camera noramlly at?*/
	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings")
		FVector NormalCamPosition;

	/*When locked on, where is the camera position at?*/
	UPROPERTY(EditDefaultsOnly, Category = "Camera Settings")
		FVector LockOnCamPosition;

	/*Camera target postion, on lock on/off, camera will move to the new position */
	UPROPERTY(BlueprintReadOnly, Category = "Camera Settings")
		FVector CameraTargetPosition;

	/*
	 * What happens when the camera locks on?
	 * Declaired as BlueprintNativeEvent since it will fun a timeline that is 
	 * much eaiser to do in Blueprint.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Camera Events")
		void OnCameraLockOn();

	/*
	* What happens when the camera locks off?
	* Declaired as BlueprintNativeEvent since it will fun a timeline that is
	* much eaiser to do in Blueprint.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Camera Events")
		void OnCameraLockOff();

	//How fast do we Sprint? Stat is added with Agility.
	UPROPERTY(EditDefaultsOnly, Category = "Sprint Settings")
		float BaseSprintSpeed;

	/*What data table is used when we want to check for level ups and extract stats from those level ups?*/
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Info")
		UDataTable* LevelUpDataTable;

	/*The level up stats, of the pawn, holds data such as CurrentStatPoints and CurrentLevelUpPoints*/
	UPROPERTY(BlueprintReadOnly, Category = "Level Up Info")
		FLevelUpStats LevelUpStats;

	/*On level up, what particle system plays?*/
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Info")
		UParticleSystem* LevelUpPS;

	/*On level up, what sound is played?*/
	UPROPERTY(EditDefaultsOnly, Category = "Level Up Info")
		USoundBase* LevelUpSound;

private: 

	/*
	* Move the pawn forward
	* @param Value: The input value of the stick
	*/
	void MoveForward(float Value);

	/*
	* Move the pawn right
	* @param Value: The input value of the stick
	*/
	void MoveRight(float Value);

	/*
	* M+K: Make the Pawn look up.
	* @param Value: The input value of the mouse
	*/
	void LookUp(float Value);

	/*
	* M+K: make the pawn look right
	* @param Value: The input value of the mouse
	*/
	void LookRight(float Value);
	
	/*
	* Controller: make the pawn look Up
	* @param Value: The input value of the stick
	*/
	void LookUpRate(float Value);

	/*
	* Controller: make the pawn look right
	* @param Value: The input value of the stick
	*/
	void LookRightRate(float Value);

	/*When the sprint button is presssed*/
	void SprintPress();
	
	/*When we start sprinting, updates movement speed*/
	void Sprint();

	/*When we stop sprinting, updates movement speed*/
	void StopSprint();

	/*When the dodge button is pressed*/
	void DodgePress();

	/*Runs a calculation to see what direction we should dodge*/
	void CalculateDodge();

	/*
	* The pawn will dodge roll in the direction given
	* @param Direction: The direction the pawn will dodge in
	*/
	void DodgeRoll(FVector Direction);

	/*Checks to see if the pawn can level up*/
	void CheckLevelUp();
	
	/*Rotate the pawn to face it's lock on target*/
	void LookAtTarget();

	/*When the lock on button is pressed*/
	void LockOnPress();

	/*Search for targets to lock onto*/
	void SearchForTargets();

	/*
	* Lock onto the incoming target and face them
	* @param Target: The incoming target to lock onto
	*/
	void LockOn(ABasePawn* Target);

	/*Stop looking at the target*/
	void LockOff();

	/*
	 * Rotate the camera left/right when locked on
	 * @param Angle: The amount the camera moves.
	 */
	void AddLockOnYaw(float Angle);

	/*
	* When looking for a new target, side scan to either the left or right
	* @param Direction: Which direction do we side scan? -1 means to the left and +1 means to the right
	*/
	void SideScanForTarget(float Direction);

	/*K+M Only, look for new targerts on the right*/
	void ScanRight();

	/*K+M Only, look for new targets on the left.*/
	void ScanLeft();
	
	/*
	* Process all the things we hit when we did lock on scanning
	* @param OutHits: A hit result of all the things we hit
	*/
	void ProcessLockOnScanHit(TArray<FHitResult> OutHits);

	/*
	*	HasLineOfSight(): First, we check to see if the test target is currently being rendered, if it is, return true.
	*	
	*	Else, it tests to make sure the player could "See" the pawn if they were to lock onto them
	*	It does a linetrace starting at the players head, and ends at the test pawn looking for world objects, if it hits a
	*	world object, we can't "see" them. Used to prevent from the player locking on a target behind walls.
	*/
	bool HasLineOfSight(ABasePawn* TestPawn) const;

	/* 
	 * When locked on, this function is called every frame, it constantly 
	 * checks to see if the camera should be moving to face the lock on target.
	 */
	void UpdateCameraLocation();

	/*
	 * When called, returns the angle between the player and their lock on, used to determine
	 * whether or not the camera should be adding pitch.
	 */
	float GetCameraAngleDelta() const;

	/*When the attack button is pressed*/
	void AttackPress();

	/*Is the pawn currently locked on?*/
	bool bIsLockedOn;

	/*Is the pawn currently Sprinting?*/
	bool bIsSprinting;

	/*
	* Returns how far we should dodge on dodge press
	* @param DodgeSpeed: The incoming dodge speed. 
	*/
	float GetDodgeStrength(float DodgeSpeed) const;

	/*In Seconds, how much time is left before stamina starts restoring?*/
	float CurrentStaminaCDSeconds;

	/*Timer handle to handle the ticking down of the CurrentStaminaCDSeconds variable*/
	FTimerHandle StaminaCDTimerHandle;

	/*Function to handle the ticking down of the CurrentStaminaCDSeconds variable*/
	void TickDownStaminaCD();

	/*Checks to see if the pawn can restore stamina*/
	bool ShouldRestoreStamina() const;

	/*
	 *Check for Direction Values, we run these pieces of code so that way we're not going at half speed when we do dodge, 
	 *Values that are greater than 0 will always result in 1 and values less than that will result in -1.
	*/
	float CorrectInputValue(float Value) const;

	/*Checks to see if the pawn can dodge*/
	bool CanDodge() const;

	/*Checks to see if the pawn can sprint*/
	bool CanSprint() const;

	/*Checks to see if the pawn has reached max level*/
	bool IsMaxLevel() const;

	/*Checks to see if the current weapons state is active*/
	bool IsAttacking() const;
	
	/*Checks to see if the current weapons state is either startup or recovery*/
	bool CanDodgeCancel() const;

	/*Checks to see if the currents weapons state is active*/
	bool CanAttackCancel() const;

	/*
	* Checks to see if the pawn is moving.
	* @param ForwardValue: The Up/Down value of the stick that determines movement
	* @param RightValue: The Left/Right value of the stick that determines movement
	*/
	bool IsMoving(float ForwardValue, float RightValue) const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/*The player camera*/
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
		class UCameraComponent* PlayerCamera;

	/*The spring arm that the camera will attach to.*/
	UPROPERTY(VisibleDefaultsOnly, Category = Boom)
		class USpringArmComponent* CameraBoom;

};
