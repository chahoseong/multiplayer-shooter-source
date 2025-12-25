#pragma once

#include "CoreMinimal.h"
#include "DustEquipmentList.h"
#include "Components/ActorComponent.h"
#include "DustEquipmentManagerComponent.generated.h"


class UDustEquipmentDefinition;
class UDustEquipmentInstance;

UCLASS()
class DUST_API UDustEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDustEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UDustEquipmentInstance* Equip(TSubclassOf<UDustEquipmentDefinition> EquipmentDefinition);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Unequip(UDustEquipmentInstance* EquipmentInstance);
	
private:
	UPROPERTY(Replicated)
	FDustEquipmentList EquipmentList;
};
