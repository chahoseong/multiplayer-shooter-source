#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DustGameplayAbility.h"
#include "DustGameplayAbility_FromEquipment.generated.h"

class UDustEquipmentInstance;

UCLASS()
class DUST_API UDustGameplayAbility_FromEquipment : public UDustGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDustGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintPure, Category="Dust|Ability")
	UDustEquipmentInstance* GetAssociatedEquipment() const;
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
