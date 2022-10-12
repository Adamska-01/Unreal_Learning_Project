#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UDEMY_GAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> HUDOverlayAsset; 
	/** Variable to hold the widget after crating it in the editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		class UUserWidget* HUDOverlay;


	/** Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> WEnemyHealthBar; 
	/** Variable to hold the widget after crating it in the editor */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		class UUserWidget* EnemyHealthBar;
	FVector EnemyLocation;
	bool bEnemyHealthBarVisible;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void DisplayEnemyHealthBar();
	void HideEnemyHealthBar();
};
