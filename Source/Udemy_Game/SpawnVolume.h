#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class UDEMY_GAME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawning)
		class UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawning)
		TSubclassOf<class ACritter> PawnToSpawn;

protected: 
	virtual void BeginPlay() override;

public:	 
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = Spawning)
		FVector GetSpawnPoint();

};
