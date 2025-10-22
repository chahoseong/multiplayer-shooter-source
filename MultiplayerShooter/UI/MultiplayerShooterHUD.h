#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiplayerShooterHUD.generated.h"

class UMultiplayerShooterUserWidget;
class UMultiplayerShooterOverlayController;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay();

	UFUNCTION(BlueprintPure)
	UMultiplayerShooterOverlayController* GetOverlayController() const;
	

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMultiplayerShooterUserWidget> OverlayWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMultiplayerShooterOverlayController> OverlayControllerClass;

	UPROPERTY()
	TObjectPtr<UMultiplayerShooterUserWidget> ActiveOverlayWidget;

	UPROPERTY()
	mutable TObjectPtr<UMultiplayerShooterOverlayController> ActiveOverlayController;
};
