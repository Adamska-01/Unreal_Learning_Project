#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Player health bar 
	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
		if (HUDOverlay != nullptr)
		{
			HUDOverlay->AddToViewport();
			HUDOverlay->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
	//Enemy health bar
	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar != nullptr)
		{
			//Show HUD in the viewport
			EnemyHealthBar->AddToViewport();
			HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
			bEnemyHealthBarVisible = false;
		}

		//Alignment (flat facing the screen (0, 0))
		FVector2D vec(0.0f, 0.0f);
		EnemyHealthBar->SetAlignmentInViewport(vec);
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar != nullptr)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 85.0f;

		FVector2D SizeInViewport = FVector2D(300.0f, 25.0f);

		//Set position and size (on screen 2D)
		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar != nullptr)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::HideEnemyHealthBar()
{
	if (EnemyHealthBar != nullptr)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
