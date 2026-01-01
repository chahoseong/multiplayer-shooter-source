#include "Player/DustPlayerState.h"
#include "AbilitySystem/DustAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DustHealthSet.h"

ADustPlayerState::ADustPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDustAbilitySystemComponent>(this, TEXT("AbilitySystem"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	HealthSet = CreateDefaultSubobject<UDustHealthSet>(TEXT("HealthSet"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
}

UAbilitySystemComponent* ADustPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UDustAbilitySystemComponent* ADustPlayerState::GetDustAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UDustHealthSet* ADustPlayerState::GetHealthSet() const
{
	return HealthSet;
}
