#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"


AFloatingPlatform::AFloatingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(Mesh); 

	StartPoint = FVector(0.0f, 0.0f, 0.0f);
	EndPoint = FVector(0.0f, 0.0f, 0.0f);

	InterpSpeed = 4.0f;
	InterpTime = 1.0f;

	bInterping = false;
}
 
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartPoint = GetActorLocation();
	EndPoint += StartPoint; //Transform to world coordinate

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

	Distance = (EndPoint - StartPoint).Size();
}
 
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bInterping)
	{
		//Interpolation
		FVector currentLocation = GetActorLocation();
		FVector interp = FMath::VInterpConstantTo(currentLocation, EndPoint, DeltaTime, (EndPoint - StartPoint).Size() * InterpSpeed);

		//Move
		SetActorLocation(interp);

		//Check if we've arrived to destination, if yes stop and swap vectors to go back after some time
		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveled <= 0.1f)
		{
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			
			SwapVectors(StartPoint, EndPoint);
		}
	}
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVectors(FVector& V1, FVector& V2)
{
	V1 = V1 + V2;
	V2 = V1 - V2;
	V1 = V1 - V2;
}

