#include "Equipment/MultiplayerShooterEquipmentManagerComponent.h"
#include "Equipment/MultiplayerShooterEquipmentDefinition.h"
#include "Equipment/MultiplayerShooterEquipmentInstance.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UMultiplayerShooterEquipmentManagerComponent::UMultiplayerShooterEquipmentManagerComponent(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UMultiplayerShooterEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	EquipmentList.OnEquipmentEntryReplicatedAdd.AddUObject(this, &ThisClass::OnEquipmentEntryReplicatedAdd);
	EquipmentList.OnEquipmentEntryReplicatedRemove.AddUObject(this, &ThisClass::OnEquipmentEntryReplicatedRemove);
}

void UMultiplayerShooterEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UMultiplayerShooterEquipmentInstance*> EquipmentInstances;

	for (const FMultiplayerShooterAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		EquipmentInstances.Add(Entry.EquipmentInstance);
	}
	
	for (UMultiplayerShooterEquipmentInstance* Instance : EquipmentInstances)
	{
		Unequip(Instance);
	}
	
	Super::UninitializeComponent();
}

void UMultiplayerShooterEquipmentManagerComponent::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

void UMultiplayerShooterEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (FMultiplayerShooterAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			if (IsValid(Entry.EquipmentInstance))
			{
				AddReplicatedSubObject(Entry.EquipmentInstance);
			}
		}
	}
}

bool UMultiplayerShooterEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool Result = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FMultiplayerShooterAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (IsValid(Entry.EquipmentInstance))
		{
			Result |= Channel->ReplicateSubobject(Entry.EquipmentInstance, *Bunch, *RepFlags);
		}
	}

	return Result;
}

UMultiplayerShooterEquipmentInstance* UMultiplayerShooterEquipmentManagerComponent::Equip(
	TSubclassOf<UMultiplayerShooterEquipmentDefinition> EquipmentDefinition)
{
	UMultiplayerShooterEquipmentInstance* NewEquipmentInstance = nullptr;
	
	if (EquipmentDefinition)
	{
		NewEquipmentInstance = EquipmentList.AddEntry(EquipmentDefinition);
		if (NewEquipmentInstance)
		{
			const UMultiplayerShooterEquipmentDefinition* EquipmentDefault =
				GetDefault<UMultiplayerShooterEquipmentDefinition>(EquipmentDefinition);
			SlotsToEquipments.Add(EquipmentDefault->EquipmentSlotTag, NewEquipmentInstance);
			
			NewEquipmentInstance->OnEquipped();
			
			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(NewEquipmentInstance);
			}
		}
	}

	return NewEquipmentInstance;
}

void UMultiplayerShooterEquipmentManagerComponent::Unequip(UMultiplayerShooterEquipmentInstance* EquipmentInstance)
{
	if (EquipmentInstance)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(EquipmentInstance);
		}
		
		EquipmentInstance->OnUnequipped();

		for (auto It = SlotsToEquipments.CreateIterator(); It; ++It)
		{
			if (It->Value == EquipmentInstance)
			{
				It.RemoveCurrent();
			}
		}
		
		EquipmentList.RemoveEntry(EquipmentInstance);
	}
}

UMultiplayerShooterEquipmentInstance* UMultiplayerShooterEquipmentManagerComponent::GetEquipmentInstance(
	const FGameplayTag& EquipmentSlotTag) const
{
	return SlotsToEquipments.FindRef(EquipmentSlotTag);
}

void UMultiplayerShooterEquipmentManagerComponent::OnEquipmentEntryReplicatedAdd(
	const FMultiplayerShooterAppliedEquipmentEntry& Entry)
{
	if (Entry.EquipmentInstance)
	{
		const UMultiplayerShooterEquipmentDefinition* EquipmentDefault =
			GetDefault<UMultiplayerShooterEquipmentDefinition>(Entry.EquipmentDefinition);
		SlotsToEquipments.Add(EquipmentDefault->EquipmentSlotTag, Entry.EquipmentInstance);

		Entry.EquipmentInstance->OnEquipped();
	}
}

void UMultiplayerShooterEquipmentManagerComponent::OnEquipmentEntryReplicatedRemove(
	const FMultiplayerShooterAppliedEquipmentEntry& Entry)
{
	if (Entry.EquipmentInstance)
	{
		Entry.EquipmentInstance->OnUnequipped();
		
		const UMultiplayerShooterEquipmentDefinition* EquipmentDefault =
			GetDefault<UMultiplayerShooterEquipmentDefinition>(Entry.EquipmentDefinition);
		SlotsToEquipments.Remove(EquipmentDefault->EquipmentSlotTag);
	}
}
