#pragma once

#include "CoreMinimal.h"
#include "Equipment/MultiplayerShooterGameplayAbility_FromEquipment.h"
#include "MultiplayerShooterGameplayAbility_RangedWeapon.generated.h"

class UMultiplayerShooterRangedWeaponInstance;

UCLASS()
class MULTIPLAYERSHOOTER_API
UMultiplayerShooterGameplayAbility_RangedWeapon : public UMultiplayerShooterGameplayAbility_FromEquipment
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="MultiplayerShooter|Ability")
	UMultiplayerShooterRangedWeaponInstance* GetWeaponInstance() const;
};
