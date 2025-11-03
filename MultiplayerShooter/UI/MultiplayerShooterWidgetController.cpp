#include "UI/MultiplayerShooterWidgetController.h"

void UMultiplayerShooterWidgetController::Initialize(APlayerController* NewPlayerController)
{
	PlayerController = NewPlayerController;	
}

APlayerState* UMultiplayerShooterWidgetController::GetPlayerState() const
{
	if (PlayerController.IsValid())
	{
		return PlayerController->PlayerState;
	}
	return nullptr;
}
