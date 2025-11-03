#include "UI/MultiplayerShooterHUD.h"
#include "UI/MultiplayerShooterUserWidget.h"
#include "UI/MultiplayerShooterOverlayController.h"


void AMultiplayerShooterHUD::InitOverlay()
{
	ActiveOverlayWidget = CreateWidget<UMultiplayerShooterUserWidget>(GetOwningPlayerController(), OverlayWidgetClass);
	UMultiplayerShooterOverlayController* OverlayController = GetOverlayController();
	ActiveOverlayWidget->SetWidgetController(OverlayController);
	OverlayController->BroadcastInitialAttributes();
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
