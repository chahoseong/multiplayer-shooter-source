#include "Equipment/MultiplayerShooterEquipmentSerializer.h"
#include "Equipment/MultiplayerShooterEquipmentDefinition.h"
#include "Equipment/MultiplayerShooterEquipmentInstance.h"
#include "AbilitySystem/MultiplayerShooterAbilitySet.h"
#include "AbilitySystem/MultiplayerShooterAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

FMultiplayerShooterEquipmentList::FMultiplayerShooterEquipmentList(UActorComponent* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
{
}

UMultiplayerShooterEquipmentInstance* FMultiplayerShooterEquipmentList::AddEntry(
	TSubclassOf<UMultiplayerShooterEquipmentDefinition> EquipmentDefinition)
{
	check(EquipmentDefinition);
	check(OwnerComponent)
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UMultiplayerShooterEquipmentDefinition* EquipmentDefault =
		GetDefault<UMultiplayerShooterEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UMultiplayerShooterEquipmentInstance> EquipmentInstanceClass = EquipmentDefault->EquipmentInstanceClass;
	if (!EquipmentInstanceClass)
	{
		EquipmentInstanceClass = UMultiplayerShooterEquipmentInstance::StaticClass();
	}
	
	FMultiplayerShooterAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.EquipmentInstance = NewObject<UMultiplayerShooterEquipmentInstance>(OwnerComponent->GetOwner(), EquipmentInstanceClass);

	// Give ability set to ability system
	if (UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent())
	{
		for (const UMultiplayerShooterAbilitySet* AbilitySet : EquipmentDefault->AbilitySets)
		{
			if (AbilitySet)
			{
				AbilitySet->GiveToAbilitySystem(
					NewEntry.GrantedHandles,
					AbilitySystem,
					NewEntry.EquipmentInstance
				);
			}
		}
	}
	
	NewEntry.EquipmentInstance->SpawnEquipmentActors(EquipmentDefault->EquipmentActorsToSpawn);
	
	MarkItemDirty(NewEntry);

	return NewEntry.EquipmentInstance;
}

void FMultiplayerShooterEquipmentList::RemoveEntry(UMultiplayerShooterEquipmentInstance* EquipmentInstance)
{
	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		FMultiplayerShooterAppliedEquipmentEntry& Entry = *It;

		if (Entry.EquipmentInstance == EquipmentInstance)
		{
			if (UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(AbilitySystem);
			}

			EquipmentInstance->DestroyEquipmentActors();

			It.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

void FMultiplayerShooterEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FMultiplayerShooterAppliedEquipmentEntry& Entry = Entries[Index];
		OnEquipmentEntryReplicatedAdd.Broadcast(Entry);
	}
}

void FMultiplayerShooterEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FMultiplayerShooterAppliedEquipmentEntry& Entry = Entries[Index];
		OnEquipmentEntryReplicatedRemove.Broadcast(Entry);
	}
}

void FMultiplayerShooterEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		const FMultiplayerShooterAppliedEquipmentEntry& Entry = Entries[Index];
		OnEquipmentEntryReplicatedChange.Broadcast(Entry);
	}
}

bool FMultiplayerShooterEquipmentList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
{
	return FastArrayDeltaSerialize<FMultiplayerShooterAppliedEquipmentEntry, FMultiplayerShooterEquipmentList>(Entries, DeltaParams, *this);
}

UAbilitySystemComponent* FMultiplayerShooterEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerComponent->GetOwner());
}

