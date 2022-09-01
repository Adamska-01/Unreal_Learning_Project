#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"
 
UCLASS()
class UDEMY_GAME_API AExplosive : public AItem
{
	GENERATED_BODY()

public:
	AExplosive();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Damage;

public:
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
