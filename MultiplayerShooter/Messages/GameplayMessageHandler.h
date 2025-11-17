#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayMessageHandler.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UGameplayMessageHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameplayMessageHandler();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void StartListening();
	virtual void StopListening();
	
protected:
	void AddListener(FGameplayMessageListenerHandle&& Handle);
	
protected:
	TArray<FGameplayMessageListenerHandle> ListenerHandles;
};
