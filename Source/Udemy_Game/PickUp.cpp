#include "PickUp.h"
#include "MainChr.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


APickUp::APickUp()
{
	CoinAmount = 1;
}

void APickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("PickUp::OnOverlapBegin()"));

	if (OtherActor)
	{
		AMainChr* main = Cast<AMainChr>(OtherActor);
		if (main)
		{
			//Spawn particles 
			if (OverlapParticles != nullptr)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorTransform());
			}
			//Play Sound 
			if (OverlapSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			main->IncrementCoin(CoinAmount);
			main->PickUpLocations.Add(GetActorLocation());
		
			Destroy();
		}
	} 
}

void APickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("PickUp::OnOverlapEnd()"));
}
