#include "Equipment/DustGameplayAbility_FromEquipment.h"
#include "Equipment/DustEquipmentInstance.h"
#include "Misc/DataValidation.h"

UDustGameplayAbility_FromEquipment::UDustGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UDustEquipmentInstance* UDustGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec())
	{
		return Cast<UDustEquipmentInstance>(AbilitySpec->SourceObject.Get());
	}
	return nullptr;
}

#if WITH_EDITOR
EDataValidationResult UDustGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		Context.AddError(NSLOCTEXT("Dust", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}
	
	return Result;
}
#endif
