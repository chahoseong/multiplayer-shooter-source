#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "MultiplayerShooterEquipmentDefinition.generated.h"

class UMultiplayerShooterAbilitySet;
class UMultiplayerShooterEquipmentInstance;

USTRUCT()
struct FMultiplayerShooterEquipmentActorToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> EquipmentActorClass;
	
	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};

UCLASS(Const, Abstract, Blueprintable, BlueprintType)
class MULTIPLAYERSHOOTER_API UMultiplayerShooterEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UMultiplayerShooterEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TSubclassOf<UMultiplayerShooterEquipmentInstance> EquipmentInstanceClass;

	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	FGameplayTag EquipmentSlotTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<FMultiplayerShooterEquipmentActorToSpawn> EquipmentActorsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<TObjectPtr<UMultiplayerShooterAbilitySet>> AbilitySets;
};
