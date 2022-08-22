#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


void UMainAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
		Pawn = TryGetPawnOwner();
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
		Pawn = TryGetPawnOwner();
	else
	{ 
		FVector velocity = Pawn->GetVelocity();

		//only if not jumping
		FVector lateralSpeed = FVector(velocity.X, velocity.Y, 0.0f);

		MovementSpeed = lateralSpeed.Size(); //The magnitude of the vector as speed
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
	}
}

