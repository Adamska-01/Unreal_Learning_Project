// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainChr.generated.h"

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
	//GETTERS AND SETTERS 
	FORCEINLINE class USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() { return FollowCamera; } 
};
