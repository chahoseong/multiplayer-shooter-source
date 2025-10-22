#include "UI/MultiplayerShooterOverlayController.h"
#include "UI/MultiplayerShooterHUD.h"
#include "UI/MultiplayerShooterUserWidget.h"

UMultiplayerShooterOverlayController* UMultiplayerShooterOverlayController::GetOverlayController(
	APlayerController* InPlayerController)
{
	if (AMultiplayerShooterHUD* HUD = InPlayerController->GetHUD<AMultiplayerShooterHUD>())
	{
		return HUD->GetOverlayController();
	}
	return nullptr;
}

void UMultiplayerShooterOverlayController::SetCrosshair(TSubclassOf<UMultiplayerShooterUserWidget> CrosshairWidgetClass)
{
	ActiveCrosshairWidget = CreateWidget<UMultiplayerShooterUserWidget>(
		PlayerController.Get(),
		CrosshairWidgetClass
	);
	ActiveCrosshairWidget->SetWidgetController(this);
	ActiveCrosshairWidget->AddToViewport();
}

void UMultiplayerShooterOverlayController::UpdateCrosshairSpread(float BaseSpreadAngle, float SpreadAngleMultiplier)
{
	OnCrosshairSpread.Broadcast(BaseSpreadAngle, SpreadAngleMultiplier);
}

void UMultiplayerShooterOverlayController::SetTarget(AActor* NewTarget)
{
	OnTargetChanged.Broadcast(NewTarget);
}
