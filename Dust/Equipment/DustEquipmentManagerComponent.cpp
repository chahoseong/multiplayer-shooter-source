#include "Equipment/DustEquipmentManagerComponent.h"
#include "DustEquipmentDefinition.h"
#include "DustEquipmentInstance.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

UDustEquipmentManagerComponent::UDustEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UDustEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, EquipmentList);
}

void UDustEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UDustEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UDustEquipmentInstance*> AllEquipmentInstances;
	
	for (const FDustAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}
	
	for (UDustEquipmentInstance* EquipmentInstance : AllEquipmentInstances)
	{
		Unequip(EquipmentInstance);
	}
	
	Super::UninitializeComponent();
}

void UDustEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
	
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FDustAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			if (IsValid(Entry.Instance))
			{
				AddReplicatedSubObject(Entry.Instance);
			}
		}
	}
}

bool UDustEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	for (FDustAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (IsValid(Entry.Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Entry.Instance, *Bunch, *RepFlags);
		}
	}
	
	return WroteSomething;
}

UDustEquipmentInstance* UDustEquipmentManagerComponent::Equip(TSubclassOf<UDustEquipmentDefinition> EquipmentDefinition)
{
	UDustEquipmentInstance* Result = nullptr;
	
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		
		if (Result)
		{
			Result->OnEquipped();
			
			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	
	return Result;
}

void UDustEquipmentManagerComponent::Unequip(UDustEquipmentInstance* EquipmentInstance)
{
	if (IsValid(EquipmentInstance))
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(EquipmentInstance);
		}
		
		EquipmentInstance->OnUnequipped();
		
		EquipmentList.RemoveEntry(EquipmentInstance);
	}
}
