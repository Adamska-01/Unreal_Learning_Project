#include "Explosive.h"
#include "MainChr.h"

AExplosive::AExplosive()
{
	Damage = 15.0f;
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapBegin()"));

	if (OtherActor)
	{
		AMainChr* main = Cast<AMainChr>(OtherActor);
		if (main)
		{
			main->DecrementHealth(Damage);
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapEnd()"));
}
