// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); //there may be crucial functionality that need to be called in the parent version of TickComponent()

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
		return;

	//Get movement vector and then consumes it
	FVector desiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f); //Normalise vector

	if (!desiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(desiredMovementThisFrame * 10.0f, UpdatedComponent->GetComponentRotation(), true, Hit);

		//if we bump into something, slide along the side of it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(desiredMovementThisFrame, 1.0f - Hit.Time, Hit.Normal, Hit);
		}
	}
}