#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MultiplayerShooterGameState.generated.h"

struct FMultiplayerShooterVerbMessage;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMultiplayerShooterGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastMessage(const FMultiplayerShooterVerbMessage& Message);
};
