#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainChr.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class UDEMY_GAME_API AMainChr : public ACharacter
{
	GENERATED_BODY()

public: 
	AMainChr();

	/** Needed to position the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true")) //meta makes it accessible in the current blueprint but not on other's blueprint
		class USpringArmComponent* CameraBoom; 
	/** Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Base turn rate to scale turning functions for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseTurnRate; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseLookUpRate;

	/**
	/* Player Stats
	**/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
		EMovementStatus MovementStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
		float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
		float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Runnnig")
		float SprintingSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		int32 Coins;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Forward/Backwards movement */
	void MoveForward(float value);
	/** Left/Right movement */
	void MoveRight(float value);

	//** Called via input to turn at a given rate
	/** @param Rate --> Normalised rate, i.e 1.0f means 100% of desired rate
	*/
	void TurnAtRate(float Rate);

	//** Called via input to turn at a given rate
	/** @param Rate --> Normalised rate, i.e 1.0f means 100% of desired look up/down rate
	*/
	void LookUpAtRate(float Rate);

public:
	/**
	/* GETTERS AND SETTERS
	**/ 
	FORCEINLINE class USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() { return FollowCamera; } 

	void DecrementHealth(float Amount);
	void IncrementCoin(int32 Amount);

	//Set Movement status and running speed
	void SetMovementStatus(EMovementStatus Status);

	void Die();
};
