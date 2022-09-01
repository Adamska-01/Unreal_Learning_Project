#include "MainChr.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	Health = 65.0f;
	MaxStamina = 350.0f;
	Stamina = 120.0f;
	Coins = 0;
}

void AMainChr::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainChr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
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

