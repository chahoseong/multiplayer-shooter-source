#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooterEquipmentSerializer.h"
#include "Components/ActorComponent.h"
#include "MultiplayerShooterEquipmentManagerComponent.generated.h"

class UMultiplayerShooterEquipmentDefinition;
class UMultiplayerShooterEquipmentInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMultiplayerShooterEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMultiplayerShooterEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void ReadyForReplication() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UMultiplayerShooterEquipmentInstance* Equip(TSubclassOf<UMultiplayerShooterEquipmentDefinition> EquipmentDefinition);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Unequip(UMultiplayerShooterEquipmentInstance* EquipmentInstance);
	
	UFUNCTION(BlueprintCallable)
	UMultiplayerShooterEquipmentInstance* GetEquipmentInstance(const FGameplayTag& EquipmentSlotTag) const;
	
	void SetEquipmentsVisibility(bool bVisible);
	
protected:
	void OnEquipmentEntryReplicatedAdd(const FMultiplayerShooterAppliedEquipmentEntry& Entry);
	void OnEquipmentEntryReplicatedRemove(const FMultiplayerShooterAppliedEquipmentEntry& Entry);
	
private:
	UPROPERTY(Replicated)
	FMultiplayerShooterEquipmentList EquipmentList;

	UPROPERTY(Transient)
	TMap<FGameplayTag, UMultiplayerShooterEquipmentInstance*> SlotsToEquipments;
};
