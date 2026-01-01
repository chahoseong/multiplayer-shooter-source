#pragma once

#include "Components/ActorComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayMessageProcessor.generated.h"

UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class DUST_API UGameplayMessageProcessor : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameplayMessageProcessor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void StartListening();
	virtual void StopListening();
	
protected:
	void AddListener(FGameplayMessageListenerHandle&& Listener);
	double GetServerTime() const;
	
private:
	TArray<FGameplayMessageListenerHandle> Listeners;
};
