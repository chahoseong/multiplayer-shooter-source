#include "Character/MultiplayerShooterHealthComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Attributes/MultiplayerShooterHealthSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/MultiplayerShooterVerbMessage.h"
#include "MultiplayerShooterGameplayTags.h"
#include "Net/UnrealNetwork.h"

UMultiplayerShooterHealthComponent* UMultiplayerShooterHealthComponent::FindHealthComponent(const AActor* Actor)
{
	return IsValid(Actor) ? Actor->FindComponentByClass<UMultiplayerShooterHealthComponent>() : nullptr;
}

UMultiplayerShooterHealthComponent::UMultiplayerShooterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	SetIsReplicatedByDefault(true);
}

void UMultiplayerShooterHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, DeathState);
}

void UMultiplayerShooterHealthComponent::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	HealthSet = AbilitySystem->GetSet<UMultiplayerShooterHealthSet>();
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::OnOutOfHealth);
}

void UMultiplayerShooterHealthComponent::OnOutOfHealth(const FGameplayEffectSpec* DamageEffectSpec)
{
#if WITH_SERVER_CODE
	UE_LOG(LogTemp, Warning, TEXT("[%s] Player Out of Health"), (GetOwner()->HasAuthority() ? TEXT("Server") : TEXT("Client")));
	FMultiplayerShooterVerbMessage Message;
	Message.Verb = MultiplayerShooterGameplayTags::Event_Player_Dead;
	Message.Instigator = DamageEffectSpec->GetEffectContext().GetEffectCauser();
	Message.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
	Message.Target = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner())->GetOwnerActor();
	Message.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(Message.Verb, Message);
#endif
}

void UMultiplayerShooterHealthComponent::ReleaseFromAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	HealthSet->OnOutOfHealth.RemoveAll(this);
}

void UMultiplayerShooterHealthComponent::StartDeath()
{
	if (DeathState != EMultiplayerShooterDeathState::NotDead)
	{
		return;
	}

	DeathState = EMultiplayerShooterDeathState::DeathStarted;
	
	if (ACharacter* OwningCharacter = GetOwner<ACharacter>())
	{
		// Disable Character Movement
		OwningCharacter->GetCharacterMovement()->DisableMovement();
		OwningCharacter->GetCharacterMovement()->StopMovementImmediately();

		if (OwningCharacter->IsLocallyControlled())
		{
			// Disable Input
			APlayerController* PlayerController = OwningCharacter->GetController<APlayerController>();
			OwningCharacter->DisableInput(PlayerController);
		}

		// Disable Collision
		OwningCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OwningCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	AActor* Owner = GetOwner();
	check(Owner);
	Owner->ForceNetUpdate();
}

void UMultiplayerShooterHealthComponent::FinishDeath()
{
	if (DeathState != EMultiplayerShooterDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EMultiplayerShooterDeathState::DeathFinished;

	AActor* Owner = GetOwner();
	check(Owner);
	Owner->ForceNetUpdate();
}

float UMultiplayerShooterHealthComponent::GetHealth() const
{
	return IsValid(HealthSet) ? HealthSet->GetHealth() : 0.0f;
}

float UMultiplayerShooterHealthComponent::GetMaxHealth() const
{
	return IsValid(HealthSet) ? HealthSet->GetMaxHealth() : 0.0f;
}

EMultiplayerShooterDeathState UMultiplayerShooterHealthComponent::GetDeathState() const
{
	return DeathState;
}

void UMultiplayerShooterHealthComponent::OnRep_DeathState(EMultiplayerShooterDeathState OldDeathState)
{
	if (OldDeathState == EMultiplayerShooterDeathState::NotDead)
	{
		if (DeathState == EMultiplayerShooterDeathState::DeathStarted)
		{
			StartDeath();
		}
		else if (DeathState == EMultiplayerShooterDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		}
	}
	else if (OldDeathState == EMultiplayerShooterDeathState::DeathStarted)
	{
		if (DeathState == EMultiplayerShooterDeathState::DeathFinished)
		{
			FinishDeath();
		}
	}
}
