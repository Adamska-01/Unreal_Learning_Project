#include "Critter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"


ACritter::ACritter()
{ 
	PrimaryActorTick.bCanEverTick = true;

	//Root component is the root of the object hierarchy
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	//Set up mesh
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());

	//Set up Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(-300.0f, 0.0f, 300.0f));
	Camera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	CurrentVelocity = FVector(0.0f, 0.0f, 0.0f);
	MaxSpeed = 100.0f;
}


void ACritter::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + ((CurrentVelocity) * DeltaTime);

	SetActorLocation(NewLocation);
}


void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACritter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACritter::MoveRight);
}

void ACritter::MoveForward(float value)
{ 
	CurrentVelocity.X = FMath::Clamp(value, -1.0f, 1.0f) * MaxSpeed; 
}

void ACritter::MoveRight(float value)
{
	CurrentVelocity.Y = FMath::Clamp(value, -1.0f, 1.0f) * MaxSpeed;
}

