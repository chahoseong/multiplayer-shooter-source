#include "Weapons/DustWeaponInstance.h"
#include "EnhancedInputSubsystems.h"
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
		// Input
		if (WeaponInputMapping)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem(OwningCharacter))
			{
				Subsystem->AddMappingContext(WeaponInputMapping, InputPriority);
			}
		}
		
		// Animation
		if (WeaponAnimLayer)
		{
			OwningCharacter->GetMesh()->LinkAnimClassLayers(WeaponAnimLayer);
		}
	}
}

void UDustWeaponInstance::OnUnequipped()
{
	if (const ACharacter* OwningCharacter = Cast<ACharacter>(GetPawn()))
	{
		// Input
		if (WeaponInputMapping)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem(OwningCharacter))
			{
				Subsystem->RemoveMappingContext(WeaponInputMapping);
			}
		}
		
		// Animation
		if (WeaponAnimLayer)
		{
			OwningCharacter->GetMesh()->UnlinkAnimClassLayers(WeaponAnimLayer);
		}
	}
	
	Super::OnUnequipped();
}

UEnhancedInputLocalPlayerSubsystem* UDustWeaponInstance::GetInputSubsystem(const ACharacter* Character) const
{
	if (const APlayerController* PlayerController = Character->GetController<APlayerController>())
	{
		return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	}
	
	return nullptr;
}
