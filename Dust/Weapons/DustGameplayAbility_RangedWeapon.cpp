#include "Weapons/DustGameplayAbility_RangedWeapon.h"
#include "Weapons/DustWeaponInstance.h"

UDustGameplayAbility_RangedWeapon::UDustGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UDustWeaponInstance* UDustGameplayAbility_RangedWeapon::GetAssociatedWeapon() const
{
	return Cast<UDustWeaponInstance>(GetAssociatedEquipment());
}
