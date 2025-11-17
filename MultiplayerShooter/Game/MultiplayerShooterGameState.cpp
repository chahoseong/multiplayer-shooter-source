#include "MultiplayerShooterGameState.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/MultiplayerShooterVerbMessage.h"

AMultiplayerShooterGameState::AMultiplayerShooterGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMultiplayerShooterGameState::MulticastMessage_Implementation(const FMultiplayerShooterVerbMessage& Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
		MessageSystem.BroadcastMessage(Message.Verb, Message);
	}
}
