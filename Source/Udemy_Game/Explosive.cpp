#include "Explosive.h"
#include "MainChr.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"


AExplosive::AExplosive()
{
	Damage = 15.0f;
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult); 

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

			//Damage player
			main->DecrementHealth(Damage);
		
			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex); 
}
