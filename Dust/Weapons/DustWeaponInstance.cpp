#include "Weapons/DustWeaponInstance.h"
#include "GameFramework/Character.h"

UDustWeaponInstance::UDustWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDustWeaponInstance::OnEquipped()
{
	Super::OnEquipped();
	
	if (const ACharacter* OwningCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (ItemAnimLayer)
		{
			OwningCharacter->GetMesh()->LinkAnimClassLayers(ItemAnimLayer);
		}
	}
}

void UDustWeaponInstance::OnUnequipped()
{
	if (const ACharacter* OwningCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (ItemAnimLayer)
		{
			OwningCharacter->GetMesh()->UnlinkAnimClassLayers(ItemAnimLayer);
		}
	}
	
	Super::OnUnequipped();
}
