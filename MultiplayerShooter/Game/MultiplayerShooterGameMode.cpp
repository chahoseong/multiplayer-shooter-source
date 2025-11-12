#include "Game/MultiplayerShooterGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MultiplayerShooterPlayerController.h"
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

void AMultiplayerShooterGameMode::EliminatePlayer(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		APawn* Pawn = PlayerController->GetPawn();
		if (IsValid(Pawn))
		{
			Pawn->Reset();
			Pawn->Destroy();
		}

		FTimerHandle RespawnTimer;
		GetWorldTimerManager().SetTimer(
			RespawnTimer,
			FTimerDelegate::CreateUObject(this, &ThisClass::OnPlayerRespawn, PlayerController),
			RespawnSeconds,
			false
		);
		RespawnTimers.Add(PlayerController, RespawnTimer);
	}
}

void AMultiplayerShooterGameMode::OnPlayerRespawn(APlayerController* PlayerController)
{
	RespawnPlayer(PlayerController);
	RespawnTimers.Remove(PlayerController);
}

void AMultiplayerShooterGameMode::RespawnPlayer(APlayerController* PlayerController)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(
		this,
		APlayerStart::StaticClass(),
		PlayerStarts
	);
	const int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
	RestartPlayerAtPlayerStart(PlayerController, PlayerStarts[Selection]);
	
	AMultiplayerShooterPlayerController* MultiplayerShooterPlayerController =
		CastChecked<AMultiplayerShooterPlayerController>(PlayerController);
	MultiplayerShooterPlayerController->OnPlayerRespawned();
}
