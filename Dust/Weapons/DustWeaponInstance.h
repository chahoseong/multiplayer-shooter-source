#pragma once

#include "CoreMinimal.h"
#include "Equipment/DustEquipmentInstance.h"
#include "DustWeaponInstance.generated.h"

UCLASS()
class DUST_API UDustWeaponInstance : public UDustEquipmentInstance
{
	GENERATED_BODY()
	
public:
	UDustWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	
protected:
	UPROPERTY(EditAnywhere, Category="Animation")
	TSubclassOf<UAnimInstance> ItemAnimLayer;
};
