#include "Floater.h"
#include "Components/StaticMeshComponent.h"
 
AFloater::AFloater()
{ 
	PrimaryActorTick.bCanEverTick = true;

	CustomStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));

	StartLocation = FVector(0.0f);
	PlacedLocation = FVector(0.0f);
	WorldOrigin = FVector(0.0f, 0.0f, 0.0f);
	InitialDirection = FVector(0.0f, 0.0f, 0.0f);
	InitialForce = FVector(20000000.0f, 0.0f, 0.0f);

	RunningTime = 0.0f;

	bInitaliseFloaterLoactions = false;
	bShouldFloat = false;

	A = 0.0f;
	B = 0.0f;
	C = 0.0f;
	D = 0.0f;
}

void AFloater::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = FVector(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f));
	StartLocation *= 150.0f; //scale

	if (bInitaliseFloaterLoactions)
	{
		SetActorLocation(StartLocation);
	}

	BaseZLocation = GetActorLocation().Z;
}

void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

	if (bShouldFloat)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = BaseZLocation + (A) * FMath::Sin(B * RunningTime - C) + D;

		SetActorLocation(NewLocation);
		
		RunningTime += DeltaTime;
	} 
}

