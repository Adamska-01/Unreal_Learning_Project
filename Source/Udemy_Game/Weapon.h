#pragma once
#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup		UMETA(DisplayName = "Pickup"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	
	EWS_MAX	UMETA(DisplayName = "Default_Max"),
};

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

	//State
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Item")
		EWeaponState WeaponState;

public:
	AWeapon();

public:
	//Callbacks
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void Equip(class AMainChr* Chr);

	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }
};
