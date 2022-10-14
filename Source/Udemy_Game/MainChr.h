#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainChr.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX"),
};
UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "Below_Minimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class UDEMY_GAME_API AMainChr : public ACharacter
{
	GENERATED_BODY()

public: 
	AMainChr();

	TArray<FVector> PickUpLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AMainPlayerController* MainPlayerController;
	 
	/** Needed to position the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess = "true")) //meta makes it accessible in the current blueprint but not on other's blueprint
		class USpringArmComponent* CameraBoom; 
	/** Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class USoundCue* HitSound;

	/** Base turn rate to scale turning functions for the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseTurnRate; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseLookUpRate;

	//Interpolation to the enemy location when attacking
	float InterpSpeed;
	bool bInterpToEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bHasCombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class AEnemy* CombatTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		FVector CombatTargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float StaminaDrainRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MinSprintStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage* CombatMontage;
	

	//Input bools 
	bool bShiftKeyDown; 
	bool bLMBDown;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
		bool bAttacking;

	/**
	/* Player Stats
	**/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
		EMovementStatus MovementStatus;	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
		EStaminaStatus StaminaStatus;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
		class AItem* ActiveOverlappingItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
		class AWeapon* EquippedWeapon;

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

	/** Pressed down to enable sprinting */
	void ShiftKeyDown();
	/** Released to stop sprinting */
	void ShiftKeyUp();

	void LMBDown();
	void LMBUp();

	virtual void Jump() override;
	  
	void Attack();
	UFUNCTION(BlueprintCallable)
		void AttackEnd(); 

	UFUNCTION(BlueprintCallable)
		void DeathEnd();

	//Debug
	UFUNCTION(BlueprintCallable)
		void ShowPickUpLocations();

public:
	/**
	/* GETTERS AND SETTERS
	**/ 
	FORCEINLINE class USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() { return FollowCamera; } 
	FORCEINLINE class AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	FRotator GetLookAtRotationYaw(FVector Target);

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; } 
	FORCEINLINE void SetEquippedWeapon(AWeapon* WeaponToSet) { EquippedWeapon = WeaponToSet; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* ItemToSet) { ActiveOverlappingItem = ItemToSet; }
	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }
	FORCEINLINE void SetInterpToEnemy(bool Interp) { bInterpToEnemy = Interp; }
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	void DecrementHealth(float Amount);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void IncrementCoin(int32 Amount);

	//Set Movement status and running speed
	void SetMovementStatus(EMovementStatus Status);

	void Die();
};
