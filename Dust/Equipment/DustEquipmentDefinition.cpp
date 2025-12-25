#include "Equipment/DustEquipmentDefinition.h"
#include "Equipment/DustEquipmentInstance.h"

UDustEquipmentDefinition::UDustEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UDustEquipmentInstance::StaticClass();
}
