#pragma once

#include "Equipment/DustGameplayAbility_FromEquipment.h"
#include "DustGameplayAbility_RangedWeapon.generated.h"

class UDustWeaponInstance;

UCLASS()
class DUST_API UDustGameplayAbility_RangedWeapon : public UDustGameplayAbility_FromEquipment
{
	GENERATED_BODY()
	
public:
	UDustGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintPure, Category="Dust|Ability")
	UDustWeaponInstance* GetAssociatedWeapon() const;
};
