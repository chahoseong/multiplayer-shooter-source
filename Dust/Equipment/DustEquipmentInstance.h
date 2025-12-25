#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DustEquipmentInstance.generated.h"

struct FDustEquipmentActorSpawnParams;

UCLASS(BlueprintType, Blueprintable)
class DUST_API UDustEquipmentInstance : public UObject
{
	GENERATED_BODY()
	
public:
	UDustEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void SpawnEquipmentActors(const TArray<FDustEquipmentActorSpawnParams>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();
	
	virtual bool IsSupportedForNetworking() const override;
	virtual UWorld* GetWorld() const override final;
	
	UFUNCTION(BlueprintPure, Category="Equipment")
	UObject* GetInstigator() const;
	
	void SetInstigator(UObject* NewInstigator);
	
	UFUNCTION(BlueprintPure, Category="Equipment")
	APawn* GetPawn() const;
	
	UFUNCTION(BlueprintPure, Category="Equipment", meta=(DeterminesOutputType=PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;
	
	UFUNCTION(BlueprintPure,  Category="Equipment")
	TArray<AActor*> GetSpawnedActors() const;
	
	virtual void OnEquipped();
	virtual void OnUnequipped();
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Equipment", meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Equipment", meta=(DisplayName="OnEquipped"))
	void K2_OnUnequipped();
	
private:
	UFUNCTION()
	void OnRep_Instigator();
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	TObjectPtr<UObject> Instigator;
	
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
