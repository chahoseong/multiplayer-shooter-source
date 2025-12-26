#pragma once

#include "CoreMinimal.h"
#include "Equipment/DustEquipmentInstance.h"
#include "DustWeaponInstance.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputMappingContext;

UCLASS()
class DUST_API UDustWeaponInstance : public UDustEquipmentInstance
{
	GENERATED_BODY()
	
public:
	UDustWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	
private:
	UEnhancedInputLocalPlayerSubsystem* GetInputSubsystem(const ACharacter* Character) const;
	
protected:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> WeaponInputMapping;
	
	UPROPERTY(EditAnywhere, Category="Input")
	int32 InputPriority = 1;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	TSubclassOf<UAnimInstance> WeaponAnimLayer;
};
