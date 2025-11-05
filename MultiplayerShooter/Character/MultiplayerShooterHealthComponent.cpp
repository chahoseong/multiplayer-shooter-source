#include "Character/MultiplayerShooterHealthComponent.h"
#include "AbilitySystem/Attributes/MultiplayerShooterHealthSet.h"

UMultiplayerShooterHealthComponent::UMultiplayerShooterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UMultiplayerShooterHealthComponent::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(
		UMultiplayerShooterHealthSet::GetHealthAttribute()
	).AddUObject(this, &ThisClass::OnHealthChanged);
}

void UMultiplayerShooterHealthComponent::ReleaseFromAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(
		UMultiplayerShooterHealthSet::GetHealthAttribute()
	).RemoveAll(this);
}

void UMultiplayerShooterHealthComponent::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue == 0)
	{
		// Dead
	}
	else if (Data.NewValue < Data.OldValue)
	{
		// Damaged
	}
	else if (Data.NewValue > Data.OldValue)
	{
		// Healing
	}
}
