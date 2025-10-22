#include "Equipment/MultiplayerShooterEquipmentInstance.h"
#include "Equipment/MultiplayerShooterEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void UMultiplayerShooterEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnedActors);
}

bool UMultiplayerShooterEquipmentInstance::IsSupportedForNetworking() const
{
	return true;
}

void UMultiplayerShooterEquipmentInstance::SpawnEquipmentActors(
	const TArray<FMultiplayerShooterEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (const ACharacter* OwningCharacter = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = OwningCharacter->GetMesh();
		}

		for (const FMultiplayerShooterEquipmentActorToSpawn& ActorToSpawn : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(
				ActorToSpawn.EquipmentActorClass,
				FTransform::Identity,
				OwningPawn
			);
			NewActor->FinishSpawning(FTransform::Identity, true);
			NewActor->SetActorRelativeTransform(ActorToSpawn.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, ActorToSpawn.AttachSocket);
			SpawnedActors.Add(NewActor);
		}
	}
}

void UMultiplayerShooterEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		Actor->Destroy();
	}
	SpawnedActors.Reset();
}

UWorld* UMultiplayerShooterEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	return nullptr;
}

APawn* UMultiplayerShooterEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UMultiplayerShooterEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			return Cast<APawn>(GetOuter());
		}
	}
	return nullptr;
}

int64 UMultiplayerShooterEquipmentInstance::GetEquipmentId() const
{
	return EquipmentId;
}

TArray<AActor*> UMultiplayerShooterEquipmentInstance::GetSpawnedActors() const
{
	return SpawnedActors;
}

void UMultiplayerShooterEquipmentInstance::OnEquipped()
{
}

void UMultiplayerShooterEquipmentInstance::OnUnequipped()
{
}
