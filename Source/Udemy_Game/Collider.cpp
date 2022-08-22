#include "Collider.h" 
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ColliderMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACollider::ACollider()
{ 
	PrimaryActorTick.bCanEverTick = true;
	 
	//Sphere/root
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent")); 
	SetRootComponent(SphereComponent); //Set the sphere as root
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());
	//Get a mesh through code
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (MeshComponentAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bEnableCameraLag = true; //smooth trailing behind pawn
	SpringArmComponent->CameraLagSpeed = 3.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	//Custom Movement component
	CustomMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("MovementComponent"));
	CustomMovementComponent->UpdatedComponent = GetRootComponent(); //Apply movement to the root component 

	//Camera Input
	CameraInput = FVector2D(0.0f, 0.0f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

  
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = GetActorRotation(); 
	NewRotation.Yaw += CameraInput.X;
	
	SetActorRotation(NewRotation);

	FRotator NewSpringArmRotation = SpringArmComponent->GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch + CameraInput.Y, - 80.0f, -15.0f);

	SpringArmComponent->SetWorldRotation(NewSpringArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

	//Camera
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawhCamera);
}

void ACollider::MoveForward(float value)
{
	FVector Forward = GetActorForwardVector();
	if (CustomMovementComponent)
	{
		CustomMovementComponent->AddInputVector(Forward * value * 40.0f);
	}
}

void ACollider::MoveRight(float value)
{
	FVector Right = GetActorRightVector();
	if (CustomMovementComponent)
	{
		CustomMovementComponent->AddInputVector(Right * value * 40.0f);
	}
}

void ACollider::PitchCamera(float value)
{
	CameraInput.Y = value;
}

void ACollider::YawhCamera(float value)
{
	CameraInput.X = value;
}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return CustomMovementComponent;
}
