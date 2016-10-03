// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG.h"
#include "Public/Items/Item.h"
#include "Public/Items/Weapon.h"
#include "Public/Items/Armor.h"
#include "Public/Items/Consumable.h"
#include "Public/Characters/Player/PlayerPawn.h"
#include "Public/Spells/BaseSpell.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PlayerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Cape = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cape"));
	Cape->SetupAttachment(GetMesh(), FName("CapeSocket"));

	bIsSprinting = false;
	bIsLockedOn = false;

	int32 CurrentStatPoints = 0;

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	InputComponent->BindAxis("LookUpRate", this, &APlayerPawn::LookUpRate);
	InputComponent->BindAxis("LookRightRate", this, &APlayerPawn::LookRightRate);
	
	/*M + K has a slightly different control scheme for handling lock on, 
	* Since it really isn;t comfortable by using straight controller controls.
	*/
	InputComponent->BindAxis("LookUp", this, &APlayerPawn::LookUp);
	InputComponent->BindAxis("LookRight", this, &APlayerPawn::LookRight);

	InputComponent->BindAction("DodgeRoll", IE_Pressed, this, &APlayerPawn::DodgePress);

	InputComponent->BindAction("ScanRight", IE_Pressed, this, &APlayerPawn::ScanRight);
	InputComponent->BindAction("ScanLeft", IE_Pressed, this, &APlayerPawn::ScanLeft);
	
	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerPawn::SprintPress);
	InputComponent->BindAction("Sprint", IE_Released, this, &APlayerPawn::StopSprint);
	
	InputComponent->BindAction("LockOn", IE_Pressed, this, &APlayerPawn::LockOnPress);
	
	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerPawn::AttackPress);

}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSprinting)
		AffectStamina(-0.1f);

	if (bIsSprinting && !CanSprint())
		StopSprint();

	if (bIsLockedOn && CurrentTarget != NULL)
		LookAtTarget();

	if (ShouldRestoreStamina())
		AffectStamina(0.1f);

}

void APlayerPawn::AffectHealth(int32 Delta, TSubclassOf<UDamgeTypeBase> DamageType)
{
	Super::AffectHealth(Delta, DamageType);
}

void APlayerPawn::AffectXP(int32 Delta)
{
	//we only want to add XP if we are not at max level yet.
	if (!IsMaxLevel())
	{
		LevelInfo.XP += Delta;
		CheckLevelUp();
	}
}

void APlayerPawn::CheckLevelUp()
{
	if (LevelUpDataTable)
	{
		const FString LevelToCheck = FString::FromInt(LevelInfo.Level + 1);
		const FName RowToFind = FName(*LevelToCheck);

		const FLevelUpData* Row = LevelUpDataTable->FindRow<FLevelUpData>(RowToFind, "None");

		//if row was valid.
		if (Row)
		{
			if (LevelInfo.XP >= Row->XPNeeded)
			{

				if (LevelUpSound)
					UGameplayStatics::PlaySoundAtLocation(this, LevelUpSound, GetActorLocation());

				if (LevelUpPS)
					UGameplayStatics::SpawnEmitterAttached(LevelUpPS, GetRootComponent());

				LevelInfo.Level++;
				LevelInfo.XP -= Row->XPNeeded; //Extra XP carries over
				LevelUpStats.CurrentStatPoints += 10;
				LevelUpStats.CurrentLevelUpPoints += 1;
				CheckLevelUp(); //We check again to see if we leveled up twice or more.
			}
		}
	}
}

void APlayerPawn::UpdateStats()
{
	Super::UpdateStats();

}

void APlayerPawn::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

	}
}

void APlayerPawn::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

	}
}

void APlayerPawn::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerPawn::LookUpRate(float Value)
{
	//Camera Pitch is enabled when locking on for testing purposes Currently.
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerPawn::LookRight(float Value)
{
	if (!bIsLockedOn)
		AddControllerYawInput(Value * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
}

void APlayerPawn::LookRightRate(float Value)
{
	if (!bIsLockedOn)
		AddControllerYawInput(Value * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
	
	else
	{
		if (Value >= 0.9f || Value <= -0.9f)
			SideScanForTarget(Value);
	}
}

void APlayerPawn::DodgePress()
{
	if (CanDodge())
	{
		//If we are currently attacking, check to see if we can dodge cancel first before dodging
		if (IsAttacking())
		{
			if (CanDodgeCancel())
			{
				StopAnimMontage(GetCurrentMontage());
				GetCurrentWeapon()->SetCurrentStatus(EWeaponStatus::WS_Idle);
				CalculateDodge();
			}
		}

		else
			CalculateDodge();
	}
}

void APlayerPawn::CalculateDodge()
{


	if (GetWorldTimerManager().IsTimerActive(StaminaCDTimerHandle))
		GetWorldTimerManager().ClearTimer(StaminaCDTimerHandle);

	FVector DodgeDir;

	const float MoveForwardValue = GetInputAxisValue("MoveForward");
	const float MoveRightValue = GetInputAxisValue("MoveRight");

	if (IsMoving(MoveForwardValue, MoveRightValue))
	{
		const FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

		const FVector ForwardDirection = GetDodgeStrength(BaseForwardDodgeSpeed) * CorrectInputValue(MoveForwardValue) * UKismetMathLibrary::GetForwardVector(Rotation);
		const FVector RightDirection = GetDodgeStrength(BaseForwardDodgeSpeed) * CorrectInputValue(MoveRightValue) * UKismetMathLibrary::GetRightVector(Rotation);

		DodgeDir = ForwardDirection + RightDirection;

		DodgeRoll(DodgeDir);
	}

	//else, jump back.
	else
	{
		DodgeDir = -1.f * GetDodgeStrength(BaseBackStepSpeed) * UKismetMathLibrary::GetForwardVector(GetActorRotation());
		DodgeRoll(DodgeDir);
	}	

	AffectStamina(-20.f);

	CurrentStaminaCDSeconds = Stamina <= 0.f ? EmptyStaminaCDSeconds : NonEmptyStaminaCDSeconds;
	GetWorldTimerManager().SetTimer(StaminaCDTimerHandle, this, &APlayerPawn::TickDownStaminaCD, 0.1f, true);
}

void APlayerPawn::DodgeRoll(FVector Direction)
{
	LaunchCharacter(Direction + FVector(0.f, 0.f, 100.f), true, false);
}

void APlayerPawn::SprintPress()
{
	if (CanSprint())
		Sprint();
}

void APlayerPawn::Sprint()
{

	if (GetWorldTimerManager().IsTimerActive(StaminaCDTimerHandle))
		GetWorldTimerManager().ClearTimer(StaminaCDTimerHandle);

	if (bIsLockedOn)
		LockOff();

	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseSprintSpeed + GetAgility();
}

void APlayerPawn::StopSprint()
{
	//Safe guard, so that way if we run out of stamina, this piece of code doesnt run twice.
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed + GetAgility();

		CurrentStaminaCDSeconds = Stamina <= 0.f ? EmptyStaminaCDSeconds : NonEmptyStaminaCDSeconds;
		GetWorldTimerManager().SetTimer(StaminaCDTimerHandle, this, &APlayerPawn::TickDownStaminaCD, 0.1f, true);
	}
}

void APlayerPawn::LockOnPress()
{
	if (!bIsLockedOn)
		SearchForTargets();

	else
		LockOff();
}

void APlayerPawn::SearchForTargets()
{
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0.f, 0.f, 10.f);

	TArray<class AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<FHitResult> OutHits;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, LockOnSearchRadius, ObjectTypes, true, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);
	
	if (Result)
		ProcessSphereScanHit(OutHits);
}

void APlayerPawn::ProcessSphereScanHit(TArray<FHitResult> OutHits)
{
	ABasePawn* CurrentClosest = nullptr;
	TArray<ABasePawn*> CheckedActors;

	for (int32 i = 0; i < OutHits.Num(); i++)
	{
		auto TestTarget = Cast<ABasePawn>(OutHits[i].GetActor());

		if (!CheckedActors.Contains(TestTarget))
		{
			CheckedActors.Add(TestTarget);

			if (i == 0)
				CurrentClosest = TestTarget;

			if (TestTarget->GetDistanceTo(this) < TestTarget->GetDistanceTo(this) && !TestTarget->GetIsDead())
				CurrentClosest = TestTarget;

		}
	}

	LockOn(CurrentClosest);
}

void APlayerPawn::ScanRight()
{
	if(bIsLockedOn)
		SideScanForTarget(1);
}

void APlayerPawn::ScanLeft()
{
	if (bIsLockedOn)
		SideScanForTarget(-1);
}

void APlayerPawn::SideScanForTarget(float Direction)
{
	for (int32 i = 0; i <= 12; i++)
	{
		FVector Start = GetActorLocation();

		//Scan Angle increases in increments of 5, up to a maximum of +/-60 degrees.
		float ScanAngle = GetActorRotation().Yaw + (i * (5 * Direction));

		FRotator ScanRotation = FRotator(GetActorRotation().Pitch, ScanAngle, GetActorRotation().Roll);
	
		FVector End = UKismetMathLibrary::GetForwardVector(ScanRotation) * SideScanDistance;
	
		TArray<class AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);

		TArray<FHitResult> OutHits;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); 

		bool Result = UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), Start, End, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);
		
		bool bShouldBreak = false;

		if (Result)
		{
			//If we have a new target...
			if(ProcessSideScanHit(OutHits))
				break;
		}
	}
}

bool APlayerPawn::ProcessSideScanHit(TArray<FHitResult> OutHits) 
{

	for (int32 i = 0; i < OutHits.Num(); i++)
	{
		if (OutHits[i].GetActor() != CurrentTarget)
		{
			ABasePawn* NewTarget = Cast<ABasePawn>(OutHits[i].GetActor());

			if (NewTarget)
			{
				SetCurrentTarget(NewTarget);
				return true;
			}
		}
	}

	return false;
}

void APlayerPawn::LockOn(ABasePawn* Target)
{
	if (bIsSprinting)
		StopSprint();

	bIsLockedOn = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetCurrentTarget(Target);
	CameraTargetPosition = LockOnCamPosition;
	OnCameraLockOn();
}

void APlayerPawn::OnCameraLockOn_Implementation()
{
}

void APlayerPawn::LockOff()
{
	bIsLockedOn = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	SetCurrentTarget(nullptr);
	CameraTargetPosition = NormalCamPosition;
	OnCameraLockOff();
}

void APlayerPawn::OnCameraLockOff_Implementation()
{
}

void APlayerPawn::LookAtTarget()
{
	if (CurrentTarget)
	{

		//If our current target is dead or out of range, LockOff
		if (CurrentTarget->GetIsDead() || CurrentTarget == NULL || CurrentTarget->GetDistanceTo(this) > LockOnSearchRadius)
			LockOff();

		//Else, face our target.
		else
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTarget->GetActorLocation());
			SetActorRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
			
			UpdateCameraLocation();
		}
	}
}

void APlayerPawn::UpdateCameraLocation()
{
	const float Angle = GetCameraAngleDelta();

	if (FMath::Abs(Angle) < 179.f)
		AddLockOnPitch(Angle);
}

void APlayerPawn::AddLockOnPitch(float Angle)
{
	const FVector PlayerForwardVector = UKismetMathLibrary::GetForwardVector(GetControlRotation()).GetSafeNormal();
	const FVector PlayerDotProduct = FVector(PlayerForwardVector.X, PlayerForwardVector.Y, 0.f);

	const FVector DifferenceInLocation = (GetActorLocation() - CurrentTarget->GetActorLocation()).GetSafeNormal();
	const FVector TargetDotProduct = FVector(DifferenceInLocation.X, DifferenceInLocation.Y, 0.f);

	const float DotProduct = FMath::Abs(UKismetMathLibrary::Dot_VectorVector(PlayerDotProduct, TargetDotProduct));

	const float PitchToAdd = Angle * (1 - DotProduct) * 1.2 * (GetWorld()->GetDeltaSeconds());

	AddControllerYawInput(PitchToAdd);

}

float APlayerPawn::GetCameraAngleDelta() const
{
	const FRotator PlayerForwardRotation = UKismetMathLibrary::GetForwardVector(GetControlRotation()).GetSafeNormal().Rotation();

	const FVector DifferenceInLocation = GetActorLocation() - CurrentTarget->GetActorLocation();

	const FRotator TargetRotation = DifferenceInLocation.GetSafeNormal().Rotation();

	const float DeltaAngle = UKismetMathLibrary::NormalizedDeltaRotator(PlayerForwardRotation, TargetRotation).Yaw;

	return DeltaAngle;
}

void APlayerPawn::AttackPress()
{
	//If we have a valid weapon
	if (GetCurrentWeapon())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		//If our AnimInstance is valid
		if (AnimInstance)
		{

			//If we have attack animations assigned to both, and we can attack 
			if (GetCurrentWeapon()->PlayerAttackData[0].AttackAnimation && GetCurrentWeapon()->PlayerAttackData[1].AttackAnimation)
			{

				const float MoveForwardValue = GetInputAxisValue("MoveForward");
				const float MoveRightValue = GetInputAxisValue("MoveRight");

				//If we are not locked on, we need to face the direction of our attack. We also check to see if the sticks have input value so that way the character doesn't jank in another
				//direction so the player can keep the direction they're currently at
				if (!bIsLockedOn && IsMoving(MoveForwardValue, MoveRightValue))
				{
						const FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

						const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(Rotation) * MoveForwardValue;
						const FVector RightDirection = UKismetMathLibrary::GetRightVector(Rotation) * MoveRightValue;

						const FVector FinalDirecion = ForwardDirection + RightDirection;

						SetActorRotation(FinalDirecion.Rotation());
				}

				//If this is our first attack
				if (!IsAttacking())
					AnimInstance->Montage_Play(GetCurrentWeapon()->PlayerAttackData[0].AttackAnimation, GetCurrentWeapon()->PlayerAttackData[0].PlayRate);

				//Rotate between attacks
				else
				{
					if (AnimInstance->Montage_IsPlaying(GetCurrentWeapon()->PlayerAttackData[0].AttackAnimation) && CanAttackCancel())
						AnimInstance->Montage_Play(GetCurrentWeapon()->PlayerAttackData[1].AttackAnimation, GetCurrentWeapon()->PlayerAttackData[1].PlayRate);
						
					else if (AnimInstance->Montage_IsPlaying(GetCurrentWeapon()->PlayerAttackData[1].AttackAnimation) && CanAttackCancel())
						AnimInstance->Montage_Play(GetCurrentWeapon()->PlayerAttackData[0].AttackAnimation, GetCurrentWeapon()->PlayerAttackData[0].PlayRate);
					
				}
			}
		}
	}
}

void APlayerPawn::TickDownStaminaCD()
{
	if (CurrentStaminaCDSeconds <= 0.f)
		GetWorldTimerManager().ClearTimer(StaminaCDTimerHandle);

	else
		CurrentStaminaCDSeconds -= 0.1f;
}

float APlayerPawn::CorrectInputValue(float Value) const
{
	/*
		When dodge rolling, if we were to do a straight multiplication of axis values, moving axis value
		half way would only do half the power. Obviously we dont want that, so we convert the axis value 
		to either full 1 or -1 depending on the axis value of the stick, to get full power.
	*/

	if (Value > 0.2f)
		return 1.f;

	else if (Value < -0.2f)
		return -1.f;

	else
		return 0.f;
}

float APlayerPawn::GetDodgeStrength(float DodgeSpeed) const
{
	return DodgeSpeed + Stats.Agility;
}

bool APlayerPawn::GetIsSprinting() const
{
	return bIsSprinting;
}

bool APlayerPawn::CanSprint() const
{
	return GetVelocity().Size() > 0.f && !GetCharacterMovement()->IsFalling() && !IsAttacking() && Stamina > 0.f;
}

bool APlayerPawn::CanDodge() const
{
	return !GetCharacterMovement()->IsFalling() && Stamina > 0.f;
}

bool APlayerPawn::IsMaxLevel() const
{
	//Greater than or equal just in case we somehow are above level 20.
	return LevelInfo.Level >= 20;
}

bool APlayerPawn::CanLevelUp() const
{
	return LevelUpStats.CurrentLevelUpPoints > 0;
}

bool APlayerPawn::IsAttacking() const
{
	return GetCurrentWeapon()->GetCurrentStatus() != EWeaponStatus::WS_Idle;
}

bool APlayerPawn::CanDodgeCancel() const
{
	return GetCurrentWeapon()->GetCurrentStatus() == EWeaponStatus::WS_Startup || GetCurrentWeapon()->GetCurrentStatus() == EWeaponStatus::WS_Recovery;
}

bool APlayerPawn::CanAttackCancel() const
{
	return GetCurrentWeapon()->GetCurrentStatus() == EWeaponStatus::WS_Recovery;
}

bool APlayerPawn::IsMoving(float ForwardValue, float RightValue) const
{
	return ForwardValue >= 0.25f || ForwardValue <= -0.25f || RightValue >= 0.25f || RightValue <= -0.25f;
}

bool APlayerPawn::ShouldRestoreStamina() const
{
	return Stamina != GetMaxStamina() && !GetWorldTimerManager().IsTimerActive(StaminaCDTimerHandle) && !bIsSprinting;
}

