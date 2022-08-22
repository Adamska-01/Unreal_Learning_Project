// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class UDEMY_GAME_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	ACollider();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* SpringArmComponent; //Needed to control camera's motion

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		class UColliderMovementComponent* CustomMovementComponent;

private:
	FVector2D CameraInput;

protected: 
	virtual void BeginPlay() override;

public:	 
	virtual void Tick(float DeltaTime) override;
	 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Getters and Setters
	FORCEINLINE class UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }
	FORCEINLINE void SetMeshComponent(class UStaticMeshComponent* Mesh) { MeshComponent = Mesh; }
	FORCEINLINE class USphereComponent* GetSphereComponent() { return SphereComponent; }
	FORCEINLINE void SetSphereComponent(class USphereComponent* sphere) { SphereComponent = sphere; }
	FORCEINLINE class UCameraComponent* GetCameraComponent() { return CameraComponent; }
	FORCEINLINE void SetCameraComponent(class UCameraComponent* cam) { CameraComponent = cam; }
	FORCEINLINE class USpringArmComponent* GetSpringarmComponent() { return SpringArmComponent; }
	FORCEINLINE void SetSpringarmComponent(class USpringArmComponent* spa) { SpringArmComponent = spa; }

	virtual UPawnMovementComponent* GetMovementComponent() const override;

private:
	void MoveForward(float value);
	void MoveRight(float value);

	void PitchCamera(float value);
	void YawhCamera(float value); 
};
