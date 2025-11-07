#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerShooterGameMode.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMultiplayerShooterGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void EliminatePlayer(APlayerController* PlayerController);
	void RespawnPlayer(APlayerController* PlayerController);

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	void OnPlayerRespawn(APlayerController* PlayerController);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float RespawnSeconds = 1.0f;
	
private:
	FGenericTeamId BlueTeamId;
	FGenericTeamId RedTeamId;
	bool bIsBlueTeamAssigned = true;

	UPROPERTY()
	TMap<TObjectPtr<APlayerController>, FTimerHandle> RespawnTimers;
};
