#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Critter.h"


ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();
	 
	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, extent);
}

void ASpawnVolume::SpawnOurPawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn != nullptr)
	{
		UWorld* world = GetWorld();
		if (world != nullptr)
		{
			ACritter* critter = world->SpawnActor<ACritter>(ToSpawn, Location, FRotator(0.0f), FActorSpawnParameters());
		}
	}
}
