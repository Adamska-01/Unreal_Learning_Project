#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainChr.h"


void UMainAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn != nullptr)
		{
			Main = Cast<AMainChr>(Pawn);
		}
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
		Pawn = TryGetPawnOwner();
	else
	{ 
		//Movement
		FVector velocity = Pawn->GetVelocity(); 
		FVector lateralSpeed = FVector(velocity.X, velocity.Y, 0.0f); //only if not jumping
		MovementSpeed = lateralSpeed.Size(); //The magnitude of the vector as speed

		//Jump/fall
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		//Re-check main for sprinting 
		if (Main == nullptr)
			Main = Cast<AMainChr>(Pawn); 
	}
}

