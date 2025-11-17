#pragma once

#include "GameplayTagContainer.h"
#include "MultiplayerShooterVerbMessage.generated.h"

USTRUCT(BlueprintType)
struct FMultiplayerShooterVerbMessage
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
	FGameplayTag Verb;
	
	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UObject> Instigator;
	
	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
	TObjectPtr<UObject> Target;
	
	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
	FGameplayTagContainer InstigatorTags;
	
	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
	FGameplayTagContainer TargetTags;
	
	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
	FGameplayTagContainer ContextTags;
	
	UPROPERTY(BlueprintReadWrite, Category="Gameplay")
	double Magnitude = 1.0;
	
	MULTIPLAYERSHOOTER_API FString ToString() const;
};
