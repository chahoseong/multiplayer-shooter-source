#include "AbilitySystem/Abilities/MultiplayerShooterGameplayAbility.h"

UMultiplayerShooterGameplayAbility::UMultiplayerShooterGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EMultiplayerShooterAbilityActivationPolicy UMultiplayerShooterGameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}
