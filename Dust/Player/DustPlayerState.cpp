#include "Player/DustPlayerState.h"
#include "AbilitySystem/DustAbilitySystemComponent.h"

ADustPlayerState::ADustPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UDustAbilitySystemComponent>(TEXT("AbilitySystem"));
}

UAbilitySystemComponent* ADustPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UDustAbilitySystemComponent* ADustPlayerState::GetDustAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
