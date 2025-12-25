#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DustAbilitySet.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Object.h"
#include "DustEquipmentList.generated.h"

class UAbilitySystemComponent;
class UDustEquipmentDefinition;
class UDustEquipmentInstance;

USTRUCT(BlueprintType)
struct FDustAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	UPROPERTY()
	TSubclassOf<UDustEquipmentDefinition> Definition;
	
	UPROPERTY()
	TObjectPtr<UDustEquipmentInstance> Instance;
	
	UPROPERTY(NotReplicated)
	FDustAbilitySet_GrantedHandles GrantedHandles;
	
	FString GetDebugString() const;
};

USTRUCT(BlueprintType)
struct FDustEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
	
	UPROPERTY()
	TArray<FDustAppliedEquipmentEntry> Entries;
	
	explicit FDustEquipmentList(UActorComponent* OwnerComponent = nullptr);
	
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams);
	
	UDustEquipmentInstance* AddEntry(TSubclassOf<UDustEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UDustEquipmentInstance* EquipmentInstance);
	
private:
	UAbilitySystemComponent* GetAbilitySystem() const;
};

template <>
struct TStructOpsTypeTraits<FDustEquipmentList> : TStructOpsTypeTraitsBase2<FDustEquipmentList>
{
	enum  { WithNetDeltaSerializer = true };
};
