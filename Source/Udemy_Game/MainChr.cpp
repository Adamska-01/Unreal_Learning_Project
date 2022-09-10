#include "MainChr.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"


AMainChr::AMainChr()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create camera boom (pulls towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f; //Camera Follows at this distance
	CameraBoom->bUsePawnControlRotation = true; //Rotate arm based on controller

	//Create Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; //DO NOT Rotate arm based on controller

	//Set capsule colider size
	GetCapsuleComponent()->SetCapsuleSize(34.0f, 88.0f, true);

	//Set turn rates for input
	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	//Set Stamina drain rates 
	StaminaDrainRate = 25.0f;
	MinSprintStamina = 50.0f;

	//Rotate only the camera/Spring arm (instead of the whole character)
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Character movement changes 
	GetCharacterMovement()->bOrientRotationToMovement = true; //Rotate to direction 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //Set a rate for the roation only on the yaw
	GetCharacterMovement()->JumpZVelocity = 650.0f; //Vertical jump velocoty 
	GetCharacterMovement()->AirControl = 0.25f; //How much control do you want in the air??

	//Setting up stats
	MaxHealth = 100.0f;
	Health = MaxHealth;
	MaxStamina = 150.0f;
	Stamina = MaxStamina;
	Coins = 0;

	//Init enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	RunningSpeed = 450.0f;
	SprintingSpeed = 650.0f;

	bShiftKeyDown = false;
	bLMBDown = false;
	bAttacking = false;
}

void AMainChr::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainChr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

	float DeltaStamina = StaminaDrainRate * DeltaTime; 
	
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown && !GetMovementComponent()->Velocity.IsZero())
		{
			if (Stamina - DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			} 
			if(!GetMovementComponent()->IsFalling()) 
				Stamina -= DeltaStamina;  
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else //Shift key up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				if (!GetMovementComponent()->IsFalling())
					Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown && !GetMovementComponent()->Velocity.IsZero())
		{ 
			if (Stamina - DeltaStamina <= 0.0f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0.0f;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else //Not Exhausted
			{
				if (!GetMovementComponent()->IsFalling())
					Stamina -= DeltaStamina;
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else //Shift key up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}
			if (!GetMovementComponent()->IsFalling())
				Stamina += DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{ 
			Stamina = 0.0f;
		}
		else //Shift key up
		{ 
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			if (!GetMovementComponent()->IsFalling())
				Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering: 
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}
		if (!GetMovementComponent()->IsFalling())
			Stamina += DeltaStamina;
		SetMovementStatus(EMovementStatus::EMS_Normal);  
		break; 
	default:
		break;
	} 
}

void AMainChr::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); //check if valid, if not it HALTS the execution	

	//Bind Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainChr::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChr::MoveRight);

	//No function specified, using the function inherited from APawn
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMainChr::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainChr::LookUpAtRate);

	//Bind Actions (There is already a function for jumping in the ACharacter class
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	//Bind Sprinting action
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AMainChr::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AMainChr::ShiftKeyUp);

	//Bind LMB action
	PlayerInputComponent->BindAction("LMB", EInputEvent::IE_Pressed, this, &AMainChr::LMBDown);
	PlayerInputComponent->BindAction("LMB", EInputEvent::IE_Released, this, &AMainChr::LMBUp);
}

void AMainChr::MoveForward(float value)
{
	if (Controller != nullptr && value != 0.0f)
	{
		//Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		//Get Forward vector
		FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //Get the x Direction vector based on the rotation
		AddMovementInput(ForwardDir, value);
	}
}

void AMainChr::MoveRight(float value)
{
	if (Controller != nullptr && value != 0.0f)
	{
		//Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		//Get Right vector
		FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //Get the x Direction vector based on the rotation
		AddMovementInput(RightDir, value);
	}
}

void AMainChr::TurnAtRate(float Rate)
{
	//Look by BaseTurnRate value every second 
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); 
}

void AMainChr::LookUpAtRate(float Rate)
{
	//Look by BaseLookUpdate value every second 
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainChr::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMainChr::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMainChr::LMBDown()
{
	bLMBDown = true;

	//Equip item by pressing LMB if a weapon pick up is overlapped
	if (ActiveOverlappingItem != nullptr)
	{
		AWeapon* weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (weapon != nullptr)
		{
			weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if (EquippedWeapon != nullptr) //Attack state
	{
		Attack();
	}
}

void AMainChr::LMBUp()
{
	bLMBDown = false;
}

void AMainChr::Attack()
{
	bAttacking = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr && CombatMontage != nullptr)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.35f);
		AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
	}
}

void AMainChr::ShowPickUpLocations()
{
	for (int i = 0; i < PickUpLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickUpLocations[i], 25.0f, 24, FLinearColor::Green, 10.0f, 0.5f); 
	}
}

void AMainChr::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.0f)
	{
		Health -= Amount;
		Die();
	}
	else
	{
		Amount = std::abs(Amount);
		Health -= Amount;
	}
}

void AMainChr::IncrementCoin(int32 Amount)
{ 
	Coins += Amount; 
}

void AMainChr::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting) 
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed; 
	else 
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed; 
}

void AMainChr::Die()
{
}

