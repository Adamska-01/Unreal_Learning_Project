#include "EnemyAnimInstance.h"
#include "Enemy.h"


void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn != nullptr)
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn != nullptr)
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
	else
	{
		//Movement
		FVector velocity = Pawn->GetVelocity();
		FVector lateralSpeed = FVector(velocity.X, velocity.Y, 0.0f); //only if not jumping
		MovementSpeed = lateralSpeed.Size(); //The magnitude of the vector as speed 
	}
}
