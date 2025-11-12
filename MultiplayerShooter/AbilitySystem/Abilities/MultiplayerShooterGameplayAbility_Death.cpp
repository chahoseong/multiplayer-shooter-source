#include "AbilitySystem/Abilities/MultiplayerShooterGameplayAbility_Death.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterGameplayTags.h"
#include "Character/MultiplayerShooterHealthComponent.h"

UMultiplayerShooterGameplayAbility_Death::UMultiplayerShooterGameplayAbility_Death(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = MultiplayerShooterGameplayTags::GameplayEvent_Dead;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UMultiplayerShooterGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                               const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
	AbilitySystem->CancelAbilities(nullptr, nullptr, this);

	SetCanBeCanceled(false);

	if (bAutoStartDeath)
	{
		StartDeath();
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UMultiplayerShooterGameplayAbility_Death::StartDeath()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	UMultiplayerShooterHealthComponent* HealthComponent =
		UMultiplayerShooterHealthComponent::FindHealthComponent(AvatarActor);
	if (IsValid(HealthComponent) && HealthComponent->GetDeathState() == EMultiplayerShooterDeathState::NotDead)
	{
		HealthComponent->StartDeath();
	}
}

void UMultiplayerShooterGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	FinishDeath();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMultiplayerShooterGameplayAbility_Death::FinishDeath()
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();
	UMultiplayerShooterHealthComponent* HealthComponent =
		UMultiplayerShooterHealthComponent::FindHealthComponent(AvatarActor);
	if (IsValid(HealthComponent) && HealthComponent->GetDeathState() == EMultiplayerShooterDeathState::DeathStarted)
	{
		HealthComponent->FinishDeath();
	}
}
