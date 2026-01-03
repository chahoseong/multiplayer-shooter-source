#include "Character/DustHealthComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DustHealthSet.h"
#include "DustGameplayTags.h"
#include "DustLogChannels.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Messages/DustMessageLibrary.h"
#include "Messages/DustVerbMessage.h"
#include "Net/UnrealNetwork.h"

UDustHealthComponent* UDustHealthComponent::FindHealthComponent(const AActor* Actor)
{
	return IsValid(Actor) ? Actor->FindComponentByClass<UDustHealthComponent>() : nullptr;
}

UDustHealthComponent::UDustHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
	SetIsReplicatedByDefault(true);
}

void UDustHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, DeathState);
}

void UDustHealthComponent::OnUnregister()
{
	if (HealthSet)
	{
		HealthSet->OnHealthChanged.RemoveAll(this);
		HealthSet->OnMaxHealthChanged.RemoveAll(this);
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}
	
	HealthSet = nullptr;
	
	Super::OnUnregister();
}

void UDustHealthComponent::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	const AActor* Owner = GetOwner();
	check(Owner);
	
	if (AbilitySystemComponent)
	{
		UE_LOG(LogDust, Error, TEXT("DustHealthComponent: Health component for owner [%s] has already  been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}
	
	AbilitySystemComponent = AbilitySystem;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogDust, Error, TEXT("DustHealthComponent: Can't initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}
	
	HealthSet = AbilitySystem->GetSet<UDustHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogDust, Error, TEXT("DustHealthComponent: Can't initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}
	
	HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
	
	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	OnMaxHealthChanged.Broadcast(this, HealthSet->GetMaxHealth(), HealthSet->GetMaxHealth(), nullptr);
}

void UDustHealthComponent::HandleHealthChanged(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, float EffectMagnitude, float OldHealth, float NewHealth)
{
	OnHealthChanged.Broadcast(this, OldHealth, NewHealth, EffectInstigator);
}

void UDustHealthComponent::HandleMaxHealthChanged(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, float EffectMagnitude, float OldMaxHealth, float NewMaxHealth)
{
	OnMaxHealthChanged.Broadcast(this, OldMaxHealth, NewMaxHealth, EffectInstigator);
}

void UDustHealthComponent::HandleOutOfHealth(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, float EffectMagnitude, float OldHealth, float NewHealth)
{
#if WITH_SERVER_CODE
	if (AbilitySystemComponent && EffectSpec)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.
		// This can be used to trigger a death gameplay ability.
		FGameplayEventData Payload;
		Payload.EventTag = DustGameplayTags::GameplayEvent_Death;
		Payload.Instigator = EffectInstigator;
		Payload.Target = AbilitySystemComponent->GetAvatarActor();
		Payload.OptionalObject = EffectSpec->Def;
		Payload.ContextHandle = EffectSpec->GetEffectContext();
		Payload.InstigatorTags = *EffectSpec->CapturedSourceTags.GetAggregatedTags();
		Payload.TargetTags = *EffectSpec->CapturedTargetTags.GetAggregatedTags();
		Payload.EventMagnitude = EffectMagnitude;
		
		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent);
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		
		// Send a standardized verb message that other systems can observe
		FDustVerbMessage Message;
		Message.Verb = DustGameplayTags::Dust_Elimination_Message;
		Message.Instigator = EffectInstigator;
		Message.InstigatorTags = *EffectSpec->CapturedSourceTags.GetAggregatedTags();
		Message.Target = UDustMessageLibrary::GetPlayerStateFromObject(AbilitySystemComponent->GetAvatarActor());
		Message.TargetTags = *EffectSpec->CapturedTargetTags.GetAggregatedTags();
		
		UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
		MessageSubsystem.BroadcastMessage(Message.Verb, Message);
	}
#endif
}

void UDustHealthComponent::StartDeath()
{
	if (DeathState != EDustDeathState::NotDead)
	{
		return;
	}
	
	DeathState = EDustDeathState::DeathStarted;
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(DustGameplayTags::Status_Death_Dying, 1);
	}
	
	AActor* Owner = GetOwner();
	check(Owner);
	
	OnDeathStarted.Broadcast(Owner);
	
	Owner->ForceNetUpdate();
}

void UDustHealthComponent::FinishDeath()
{
	if (DeathState != EDustDeathState::DeathStarted)
	{
		return;
	}
	
	DeathState = EDustDeathState::DeathFinished;
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(DustGameplayTags::Status_Death_Dead, 1);
	}
	
	AActor* Owner = GetOwner();
	check(Owner);
	
	OnDeathFinished.Broadcast(Owner);
	
	Owner->ForceNetUpdate();
}

float UDustHealthComponent::GetHealth() const
{
	return HealthSet ? HealthSet->GetHealth() : 0.0f;
}

float UDustHealthComponent::GetMaxHealth() const
{
	return HealthSet ? HealthSet->GetMaxHealth() : 0.0f;
}

EDustDeathState UDustHealthComponent::GetDeathState() const
{
	return DeathState;
}

bool UDustHealthComponent::IsDeadOrDying() const
{
	return DeathState != EDustDeathState::NotDead;
}

void UDustHealthComponent::OnRep_DeathState(EDustDeathState OldDeathState)
{
	const EDustDeathState NewDeathState = DeathState;
	
	DeathState = OldDeathState;
	
	// 클라이언트의 Death State가 서버에서 복제한 Death State보다 더 진행했다면(클라이언트 예측)
	// 더이상 진행하지 않습니다.
	if (OldDeathState > NewDeathState)
	{
		UE_LOG(LogDust, Warning, TEXT("DustHealthComponent: Predicted past server death state [%d] -> [%d] for owner [%s]."),
			static_cast<uint8>(OldDeathState), static_cast<uint8>(NewDeathState), *GetNameSafe(GetOwner()));
		return;
	}
	
	if (OldDeathState == EDustDeathState::NotDead)
	{
		if (NewDeathState == EDustDeathState::DeathStarted)
		{
			StartDeath();
		}
		else if (NewDeathState == EDustDeathState::DeathFinished)
		{
			StartDeath();
			FinishDeath();
		}
		else
		{
			UE_LOG(LogDust, Error, TEXT("DustHealthComponent: Invalid death transition [%d] -> [%d] for owner [%s]."),
				static_cast<uint8>(OldDeathState), static_cast<uint8>(NewDeathState), *GetNameSafe(GetOwner()));
		}
	}
}