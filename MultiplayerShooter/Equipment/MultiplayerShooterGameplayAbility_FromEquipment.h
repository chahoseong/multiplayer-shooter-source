#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MultiplayerShooterGameplayAbility.h"
#include "MultiplayerShooterGameplayAbility_FromEquipment.generated.h"

class UMultiplayerShooterEquipmentInstance;

UCLASS()
class MULTIPLAYERSHOOTER_API
UMultiplayerShooterGameplayAbility_FromEquipment : public UMultiplayerShooterGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="MultiplayerShooter|Ability")
	UMultiplayerShooterEquipmentInstance* GetAssociatedEquipment() const;
};
