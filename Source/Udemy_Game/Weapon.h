#pragma once
#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


UCLASS()
class UDEMY_GAME_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BLueprintReadWrite, Category = "SkeletalMesh") 
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Item | Sound")
		class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Item | Particles")
		bool bWeaponParticle;

public:
	AWeapon();

public:
	//Callbacks
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void Equip(class AMainChr* Chr);
};
