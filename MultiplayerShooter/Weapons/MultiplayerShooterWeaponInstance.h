#pragma once

#include "CoreMinimal.h"
#include "Equipment/MultiplayerShooterEquipmentInstance.h"
#include "MultiplayerShooterWeaponInstance.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterWeaponInstance : public UMultiplayerShooterEquipmentInstance
{
	GENERATED_BODY()

public:
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TSubclassOf<UAnimInstance> EquipmentLayer;
};
