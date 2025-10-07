#include "Equipment/MultiplayerShooterGameplayAbility_FromEquipment.h"
#include "Equipment/MultiplayerShooterEquipmentInstance.h"

UMultiplayerShooterEquipmentInstance* UMultiplayerShooterGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec())
	{
		return Cast<UMultiplayerShooterEquipmentInstance>(AbilitySpec->SourceObject.Get());
	}
	return nullptr;
}
