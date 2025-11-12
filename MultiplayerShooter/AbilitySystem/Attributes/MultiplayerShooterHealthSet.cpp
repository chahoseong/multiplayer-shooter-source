#include "AbilitySystem/Attributes/MultiplayerShooterHealthSet.h"
#include "GameplayEffectExtension.h"
#include "MultiplayerShooterGameplayTags.h"
#include "Net/UnrealNetwork.h"

void UMultiplayerShooterHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UMultiplayerShooterHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	NewValue = ClampAttribute(Attribute, NewValue);
}

void UMultiplayerShooterHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
                                                       float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > NewValue)
		{
			UAbilitySystemComponent* AbilitySystem = GetOwningAbilitySystemComponent();
			check(AbilitySystem);
			AbilitySystem->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfHealth && GetHealth() > 0.0f)
	{
		bOutOfHealth = false;
	}
}

bool UMultiplayerShooterHealthSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();
	
	return true;
}

void UMultiplayerShooterHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float IncomingDamage = GetDamage();
		SetDamage(0.0f);

		const float NewHealth = GetHealth() - IncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		
		if (IncomingDamage > 0.0f && !bOutOfHealth)
		{
			UAbilitySystemComponent* AbilitySystem = GetOwningAbilitySystemComponentChecked();
			FGameplayEventData Payload = MakeGameplayEventData(Data.EffectSpec, IncomingDamage);
			if (GetHealth() > 0.0f)
			{
				Payload.EventTag = MultiplayerShooterGameplayTags::GameplayEvent_Damaged;
			}
			else
			{
				Payload.EventTag = MultiplayerShooterGameplayTags::GameplayEvent_Dead;
			}
			FScopedPredictionWindow NewScopedWindow(AbilitySystem, true);
			AbilitySystem->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
	}
	
	bOutOfHealth = GetHealth() <= 0.0f;
}

FGameplayEventData UMultiplayerShooterHealthSet::MakeGameplayEventData(
	const FGameplayEffectSpec& EffectSpec, float Magnitude) const
{
	FGameplayEventData Payload;
	Payload.Instigator = EffectSpec.GetEffectContext().GetInstigator();
	Payload.Target = GetOwningAbilitySystemComponent()->GetAvatarActor();
	Payload.OptionalObject = EffectSpec.Def;
	Payload.ContextHandle = EffectSpec.GetContext();
	Payload.InstigatorTags = *EffectSpec.CapturedSourceTags.GetAggregatedTags();
	Payload.TargetTags = *EffectSpec.CapturedTargetTags.GetAggregatedTags();
	Payload.EventMagnitude = Magnitude;
	return Payload;
}

void UMultiplayerShooterHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth);
}

void UMultiplayerShooterHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth);
}

float UMultiplayerShooterHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		return FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		return FMath::Max(NewValue, 1.0f);
	}

	return NewValue;
}
