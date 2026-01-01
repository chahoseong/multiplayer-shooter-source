#include "AbilitySystem/Attributes/DustAttributeSet.h"
#include "AbilitySystem/DustAbilitySystemComponent.h"

UWorld* UDustAttributeSet::GetWorld() const
{
	return Super::GetWorld();
}

UDustAbilitySystemComponent* UDustAttributeSet::GetDustAbilitySystemComponent() const
{
	return Cast<UDustAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
