#include "AbilitySystem/Attributes/MultiplayerShooterCombatSet.h"
#include "Net/UnrealNetwork.h"

void UMultiplayerShooterCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, AttackDamage, COND_OwnerOnly, REPNOTIFY_Always);
}

void UMultiplayerShooterCombatSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, AttackDamage, OldAttackDamage);
}
