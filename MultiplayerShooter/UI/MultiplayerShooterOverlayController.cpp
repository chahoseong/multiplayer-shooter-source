#include "UI/MultiplayerShooterOverlayController.h"

#include "Player/MultiplayerShooterPlayerController.h"
#include "UI/MultiplayerShooterHUD.h"
#include "UI/MultiplayerShooterUserWidget.h"

UMultiplayerShooterOverlayController* UMultiplayerShooterOverlayController::GetOverlayController(APlayerController* InPlayerController)
{
	if (const AMultiplayerShooterHUD* HUD = InPlayerController->GetHUD<AMultiplayerShooterHUD>())
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

void UMultiplayerShooterOverlayController::SetTarget(const AActor* NewTarget, ETeamAttitude::Type Attitude) const
{
	OnTargetChanged.Broadcast(NewTarget, Attitude);
}
