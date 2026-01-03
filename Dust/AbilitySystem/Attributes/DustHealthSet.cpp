#include "AbilitySystem/Attributes/DustHealthSet.h"

#include "DustGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messages/DustVerbMessage.h"
#include "Net/UnrealNetwork.h"

void UDustHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UDustHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	NewValue = ClampAttribute(Attribute, NewValue);
}

void UDustHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMaxHealthAttribute())
	{
		// 최대 체력이 변경됐을 때, 기존 체력이 변경된 최대 체력보다 크지 않도록 수정합니다.
		if (GetHealth() > NewValue)
		{
			UAbilitySystemComponent* AbilitySystem = GetOwningAbilitySystemComponentChecked();
			AbilitySystem->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
	
	if (bOutOfHealth && GetHealth() > 0.0f)
	{
		bOutOfHealth = false;
	}
}

bool UDustHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();
	
	return true;
}

void UDustHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float IncomingDamage = Data.EvaluatedData.Magnitude;
		SetDamage(0.0f);
		
		if (IncomingDamage > 0.0f)
		{
			FDustVerbMessage Message;
			Message.Verb = DustGameplayTags::Dust_Damage_Message;
			Message.Instigator = Causer;
			Message.InstigatorTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
			Message.Target = GetOwningActor();
			Message.TargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();
			Message.Magnitude = IncomingDamage;
			
			UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSubsystem.BroadcastMessage(Message.Verb, Message);
		}
		
		const float NewHealth = FMath::Clamp(GetHealth() - IncomingDamage, 0.0f, GetMaxHealth());
		SetHealth(NewHealth);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		OnMaxHealthChanged.Broadcast(
			Instigator, 
			Causer, 
			&Data.EffectSpec,
			Data.EvaluatedData.Magnitude,
			MaxHealthBeforeAttributeChange,
			GetMaxHealth()
		);
	}
	
	if (GetHealth() != HealthBeforeAttributeChange)
	{
		OnHealthChanged.Broadcast(
			Instigator,
			Causer,
			&Data.EffectSpec,
			Data.EvaluatedData.Magnitude,
			HealthBeforeAttributeChange,
			GetHealth()
		);
	}
	
	if (GetHealth() <= 0.0f && !bOutOfHealth)
	{
		OnOutOfHealth.Broadcast(
			Instigator,
			Causer,
			&Data.EffectSpec,
			Data.EvaluatedData.Magnitude,
			HealthBeforeAttributeChange,
			GetHealth()
		);
	}
	
	bOutOfHealth = GetHealth() <= 0.0f;
}

float UDustHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float Value) const
{
	if (Attribute == GetHealthAttribute())
	{
		return FMath::Clamp(Value, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		return FMath::Max(Value, 1.0f);
	}
	
	return Value;
}

void UDustHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth);
	
	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldHealth.GetCurrentValue();
	
	OnHealthChanged.Broadcast(
		nullptr, /* EffectInstigator */
		nullptr, /* EffectCauser */
		nullptr, /* EffectSpec */
		EstimatedMagnitude,
		OldHealth.GetCurrentValue(),
		CurrentHealth
	);
	
	if (!bOutOfHealth && CurrentHealth <= 0.0f)
	{
		OnOutOfHealth.Broadcast(
			nullptr,
			nullptr,
			nullptr,
			EstimatedMagnitude,
			OldHealth.GetCurrentValue(),
			CurrentHealth
		);
	}
	
	bOutOfHealth = CurrentHealth <= 0.0f;
}

void UDustHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth);
	
	OnMaxHealthChanged.Broadcast(
		nullptr,
		nullptr,
		nullptr,
		GetMaxHealth() - OldMaxHealth.GetCurrentValue(),
		OldMaxHealth.GetCurrentValue(),
		GetMaxHealth()
	);
}
