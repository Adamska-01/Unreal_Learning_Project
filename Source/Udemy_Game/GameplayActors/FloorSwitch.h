#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class UDEMY_GAME_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloorSwitch();

	/** Overlap volume for dunctionality to be triuggered */
	UPROPERTY(VisibleAnywhere, BLueprintReadWrite, Category = "Floor Switch")
		class UBoxComponent* TriggerBox;

	/** Switch for the character to step on */
	UPROPERTY(VisibleAnywhere, BLueprintReadWrite, Category = "Floor Switch")
		class UStaticMeshComponent* FloorSwitch;

	/** Reference to the door to move when the switch is triggered */
	UPROPERTY(VisibleAnywhere, BLueprintReadWrite, Category = "Floor Switch")
		class UStaticMeshComponent* Door;

	/** Inital door location */
	UPROPERTY(BLueprintReadWrite, Category = "Floor Switch")
		FVector InitialDoorLocation;

	/** Inital switch location */
	UPROPERTY(BLueprintReadWrite, Category = "Floor Switch")
		FVector InitialSwitchLocation;

	/** Handle to the timer struct */
	FTimerHandle SwitchHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Switch")
		float SwitchTime;

private:
	bool bCharacterOnSwitch;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//BlueprintImplementableEvent: You do not necessarely have to implement a functionality here, you can also 
	//do it in the blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = Floor_Switch)
		void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = Floor_Switch)
		void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = Floor_Switch)
		void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = Floor_Switch)
		void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable, Category = Floor_Switch)
		void UpdateDoorLocation(float Z);

	UFUNCTION(BlueprintCallable, Category = Floor_Switch)
		void UpdateFloorSwitchLocation(float Z);

	void CloseDoor();
};
