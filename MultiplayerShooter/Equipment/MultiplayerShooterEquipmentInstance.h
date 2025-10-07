#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MultiplayerShooterEquipmentInstance.generated.h"

class UMultiplayerShooterEquipmentDefinition;
struct FMultiplayerShooterEquipmentActorToSpawn;

UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERSHOOTER_API UMultiplayerShooterEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;
	
	virtual void SpawnEquipmentActors(const TArray<FMultiplayerShooterEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintPure, Category="Equipment")
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category="Equipment")
	int64 GetEquipmentId() const;

	UFUNCTION(BlueprintPure, Category="Equipment")
	TArray<AActor*> GetSpawnedActors() const;

	virtual void OnEquipped();
	virtual void OnUnequipped();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int64 EquipmentId;

private:
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
