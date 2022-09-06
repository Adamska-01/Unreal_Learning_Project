#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class UDEMY_GAME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	/** Base shape collision */
	UPROPERTY(VisibleAnywhere, BLueprintReadWrite, Category = "Item | Collision")
		class USphereComponent* CollisionVolume;

	/** Base Mesh component */
	UPROPERTY(VisibleAnywhere, BLueprintReadWrite, Category = "Item | Mesh")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Item | Particles")
		class UParticleSystemComponent* IdleParticlesComponent;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Item | Particles")
		class UParticleSystem* OverlapParticles;
	  
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Item | Sound")
		class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Item | Properties")
		bool bRotate;
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Item | Properties")
		float RotationRate;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
