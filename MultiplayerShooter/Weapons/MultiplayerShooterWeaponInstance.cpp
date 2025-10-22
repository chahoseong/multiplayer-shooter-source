#include "Weapons/MultiplayerShooterWeaponInstance.h"
#include "GameFramework/Character.h"

void UMultiplayerShooterWeaponInstance::OnEquipped()
{
	Super::OnEquipped();

	if (ACharacter* OwningCharacter = GetPawn<ACharacter>())
	{
		OwningCharacter->GetMesh()->LinkAnimClassLayers(EquipmentLayer);
	}
}

void UMultiplayerShooterWeaponInstance::OnUnequipped()
{
	if (ACharacter* OwningCharacter = GetPawn<ACharacter>())
	{
		OwningCharacter->GetMesh()->UnlinkAnimClassLayers(EquipmentLayer);
	}
	
	Super::OnUnequipped();
}
