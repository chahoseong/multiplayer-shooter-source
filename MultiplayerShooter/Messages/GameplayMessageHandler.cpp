#include "Messages/GameplayMessageHandler.h"
#include "GameFramework/GameplayMessageSubsystem.h"

UGameplayMessageHandler::UGameplayMessageHandler()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameplayMessageHandler::BeginPlay()
{
	Super::BeginPlay();
	StartListening();
}

void UGameplayMessageHandler::StartListening()
{
}

void UGameplayMessageHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopListening();
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	for (const FGameplayMessageListenerHandle& Handle : ListenerHandles)
	{
		MessageSubsystem.UnregisterListener(Handle);
	}
	ListenerHandles.Empty();
	
	Super::EndPlay(EndPlayReason);
}

void UGameplayMessageHandler::StopListening()
{
}

void UGameplayMessageHandler::AddListener(FGameplayMessageListenerHandle&& Handle)
{
	ListenerHandles.Add(MoveTemp(Handle));
}
