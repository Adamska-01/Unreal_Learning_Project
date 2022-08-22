// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Critter.generated.h"

UCLASS()
class UDEMY_GAME_API ACritter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACritter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Pawn Movement")
		float MaxSpeed;
private:
	FVector CurrentVelocity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float value);
	void MoveRight(float value);

};
