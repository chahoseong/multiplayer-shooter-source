#include "Weapons/MultiplayerShooterGameplayAbility_RangedWeapon.h"
#include "Weapons/MultiplayerShooterRangedWeaponInstance.h"

UMultiplayerShooterRangedWeaponInstance* UMultiplayerShooterGameplayAbility_RangedWeapon::GetWeaponInstance() const
{
	return Cast<UMultiplayerShooterRangedWeaponInstance>(GetAssociatedEquipment());
}
