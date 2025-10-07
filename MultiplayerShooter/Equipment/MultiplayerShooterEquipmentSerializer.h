#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MultiplayerShooterAbilitySet.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Object.h"
#include "MultiplayerShooterEquipmentSerializer.generated.h"

class UMultiplayerShooterEquipmentDefinition;
class UMultiplayerShooterEquipmentInstance;

USTRUCT()
struct FMultiplayerShooterAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UMultiplayerShooterEquipmentDefinition> EquipmentDefinition;
	
	UPROPERTY()
	TObjectPtr<UMultiplayerShooterEquipmentInstance> EquipmentInstance;

	UPROPERTY(NotReplicated)
	FMultiplayerShooterAbilitySet_GrantedHandles GrantedHandles;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentEntryReplicated, const FMultiplayerShooterAppliedEquipmentEntry&)

USTRUCT()
struct FMultiplayerShooterEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FMultiplayerShooterAppliedEquipmentEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;

	FMultiplayerShooterEquipmentList(UActorComponent* InOwnerComponent = nullptr);

	UMultiplayerShooterEquipmentInstance* AddEntry(TSubclassOf<UMultiplayerShooterEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UMultiplayerShooterEquipmentInstance* EquipmentInstance);
	
	//~Begin FFastArraySerializer contract
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams);

	FOnEquipmentEntryReplicated OnEquipmentEntryReplicatedAdd;
	FOnEquipmentEntryReplicated OnEquipmentEntryReplicatedRemove;
	FOnEquipmentEntryReplicated OnEquipmentEntryReplicatedChange;
	
protected:
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
};

template <>
struct TStructOpsTypeTraits<FMultiplayerShooterEquipmentList> : TStructOpsTypeTraitsBase2<FMultiplayerShooterEquipmentList>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};