#include "UI/MultiplayerShooterHUD.h"
#include "UI/MultiplayerShooterUserWidget.h"
#include "UI/MultiplayerShooterOverlayController.h"


void AMultiplayerShooterHUD::InitOverlay()
{
	ActiveOverlayWidget = CreateWidget<UMultiplayerShooterUserWidget>(GetOwningPlayerController(), OverlayWidgetClass);
	ActiveOverlayWidget->SetWidgetController(GetOverlayController());
	ActiveOverlayWidget->AddToViewport();
}

UMultiplayerShooterOverlayController* AMultiplayerShooterHUD::GetOverlayController() const
{
	if (!ActiveOverlayController)
	{
		ActiveOverlayController = NewObject<UMultiplayerShooterOverlayController>();
		ActiveOverlayController->Initialize(GetOwningPlayerController());
	}
	return ActiveOverlayController;
}
