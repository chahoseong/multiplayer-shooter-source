#include "GameplayMessageProcessor.h"
#include "GameFramework/GameStateBase.h"

UGameplayMessageProcessor::UGameplayMessageProcessor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameplayMessageProcessor::BeginPlay()
{
	Super::BeginPlay();
	StartListening();
}

void UGameplayMessageProcessor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopListening();
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	for (const FGameplayMessageListenerHandle& Listener : Listeners)
	{
		MessageSubsystem.UnregisterListener(Listener);
	}
	Listeners.Empty();
	
	Super::EndPlay(EndPlayReason);
}

void UGameplayMessageProcessor::StartListening()
{
}

void UGameplayMessageProcessor::StopListening()
{
}

void UGameplayMessageProcessor::AddListener(FGameplayMessageListenerHandle&& Listener)
{
	Listeners.Add(MoveTemp(Listener));
}

double UGameplayMessageProcessor::GetServerTime() const
{
	if (const AGameStateBase* GameState = GetWorld()->GetGameState())
	{
		return GameState->GetServerWorldTimeSeconds();
	}
	return 0.0f;
}
