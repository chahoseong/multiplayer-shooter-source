#include "Player/MultiplayerShooterPlayerState.h"
#include "AbilitySystem/MultiplayerShooterAbilitySystemComponent.h"

AMultiplayerShooterPlayerState::AMultiplayerShooterPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMultiplayerShooterAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	SetNetUpdateFrequency(100.0f);
}

UAbilitySystemComponent* AMultiplayerShooterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AMultiplayerShooterPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
