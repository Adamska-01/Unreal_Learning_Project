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
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/** Variable to hold the widget after crating it in the editor */
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Widgets")
		class UUserWidget* HUDOverlay;

protected:
	virtual void BeginPlay() override;
};
