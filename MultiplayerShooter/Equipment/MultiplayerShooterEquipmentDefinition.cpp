#include "Equipment/MultiplayerShooterEquipmentDefinition.h"
#include "Equipment/MultiplayerShooterEquipmentInstance.h"

UMultiplayerShooterEquipmentDefinition::UMultiplayerShooterEquipmentDefinition(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EquipmentInstanceClass = UMultiplayerShooterEquipmentInstance::StaticClass();
}
