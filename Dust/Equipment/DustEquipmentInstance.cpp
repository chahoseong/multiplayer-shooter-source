#include "Equipment/DustEquipmentInstance.h"
#include "Equipment/DustEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UDustEquipmentInstance::UDustEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDustEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

void UDustEquipmentInstance::SpawnEquipmentActors(const TArray<FDustEquipmentActorSpawnParams>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* TargetToAttach = OwningPawn->GetRootComponent();
		if (const ACharacter* OwningCharacter = Cast<ACharacter>(OwningPawn))
		{
			TargetToAttach = OwningCharacter->GetMesh();
		}
		
		for (const FDustEquipmentActorSpawnParams& Params :  ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(
				Params.ActorToSpawn,
				FTransform::Identity,
				OwningPawn
			);
			NewActor->FinishSpawning(FTransform::Identity, true);
			NewActor->SetActorRelativeTransform(Params.TransformToAttach);
			NewActor->AttachToComponent(TargetToAttach, FAttachmentTransformRules::KeepRelativeTransform, Params.SocketToAttach);
			SpawnedActors.Add(NewActor);
		}
	}
}

void UDustEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (IsValid(Actor))
		{
			Actor->Destroy();
		}
	}
}

bool UDustEquipmentInstance::IsSupportedForNetworking() const
{
	return true;
}

UWorld* UDustEquipmentInstance::GetWorld() const
{
	if (const APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	return nullptr;
}

UObject* UDustEquipmentInstance::GetInstigator() const
{
	return Instigator;
}

void UDustEquipmentInstance::SetInstigator(UObject* NewInstigator)
{
	Instigator = NewInstigator;
}

APawn* UDustEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UDustEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	
	return Result;
}

TArray<AActor*> UDustEquipmentInstance::GetSpawnedActors() const
{
	return SpawnedActors;
}

void UDustEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UDustEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

void UDustEquipmentInstance::OnRep_Instigator()
{
}
