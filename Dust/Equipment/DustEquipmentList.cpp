#include "Equipment/DustEquipmentList.h"
#include "Equipment/DustEquipmentDefinition.h"
#include "Equipment/DustEquipmentInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

/**
 * FDustAppliedEquipmentEntry 
 */

FString FDustAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(Definition.Get()));
}

/**
 * FDustAppliedEquipmentList
 */

FDustEquipmentList::FDustEquipmentList(UActorComponent* OwnerComponent)
	: OwnerComponent(OwnerComponent)
{
}

void FDustEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 Index : RemovedIndices)
	{
		const FDustAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FDustEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Index : AddedIndices)
	{
		const FDustAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FDustEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	
}

bool FDustEquipmentList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
{
	return FastArrayDeltaSerialize<FDustAppliedEquipmentEntry, FDustEquipmentList>(Entries, DeltaParams, *this);
}

UDustEquipmentInstance* FDustEquipmentList::AddEntry(TSubclassOf<UDustEquipmentDefinition> EquipmentDefinition)
{
	check(EquipmentDefinition);
	check(OwnerComponent && OwnerComponent->GetOwnerRole() == ROLE_Authority);
	
	const UDustEquipmentDefinition* EquipmentDefault = GetDefault<UDustEquipmentDefinition>(EquipmentDefinition);
	
	TSubclassOf<UDustEquipmentInstance> InstanceType = EquipmentDefault->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UDustEquipmentInstance::StaticClass();
	}
	
	FDustAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Definition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UDustEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	
	if (UAbilitySystemComponent* AbilitySystem = GetAbilitySystem())
	{
		for (const TObjectPtr<const UDustAbilitySet>& AbilitySet : EquipmentDefault->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystem, &NewEntry.GrantedHandles, NewEntry.Instance);
		}
	}
	
	NewEntry.Instance->SpawnEquipmentActors(EquipmentDefault->ActorsToSpawn);
	
	MarkItemDirty(NewEntry);
	
	return NewEntry.Instance;
}

void FDustEquipmentList::RemoveEntry(UDustEquipmentInstance* EquipmentInstance)
{
	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		FDustAppliedEquipmentEntry& Entry = *It;
		if (Entry.Instance == EquipmentInstance)
		{
			if (UAbilitySystemComponent* AbilitySystem = GetAbilitySystem())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(AbilitySystem);
			}
			
			EquipmentInstance->DestroyEquipmentActors();
			
			It.RemoveCurrent();
			
			MarkArrayDirty();
		}
	}
}

UAbilitySystemComponent* FDustEquipmentList::GetAbilitySystem() const
{
	check(OwnerComponent);
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComponent->GetOwner());
}
