#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DustEquipmentDefinition.generated.h"

class UDustAbilitySet;
class UDustEquipmentInstance;

USTRUCT()
struct FDustEquipmentActorSpawnParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	TSubclassOf<AActor> ActorToSpawn;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	FName SocketToAttach;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	FTransform TransformToAttach;
};

UCLASS(Const, Abstract, Blueprintable, BlueprintType)
class DUST_API UDustEquipmentDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UDustEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TSubclassOf<UDustEquipmentInstance>  InstanceType;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<TObjectPtr<const UDustAbilitySet>> AbilitySetsToGrant;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<FDustEquipmentActorSpawnParams> ActorsToSpawn;
};
