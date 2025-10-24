#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerShooterGameMode.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerShooterGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	FGenericTeamId BlueTeamId;
	FGenericTeamId RedTeamId;
	bool bIsBlueTeamAssigned = true;
};
