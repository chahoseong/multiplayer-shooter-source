#include "Game/MultiplayerShooterGameMode.h"
#include "Player/MultiplayerShooterPlayerState.h"

AMultiplayerShooterGameMode::AMultiplayerShooterGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BlueTeamId = 1;
	RedTeamId = 2;
}

void AMultiplayerShooterGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AMultiplayerShooterPlayerState* PlayerState =
		NewPlayer->GetPlayerState<AMultiplayerShooterPlayerState>();
	if (bIsBlueTeamAssigned)
	{
		PlayerState->SetGenericTeamId(BlueTeamId);
	}
	else
	{
		PlayerState->SetGenericTeamId(RedTeamId);
	}

	bIsBlueTeamAssigned = !bIsBlueTeamAssigned;
}
