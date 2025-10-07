#include "AbilitySystem/MultiplayerShooterAbilitySet.h"
#include "AbilitySystemComponent.h"

void FMultiplayerShooterAbilitySet_GrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);

	if (!AbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}

	// Clear the granted gameplay abilities
	for (FGameplayAbilitySpecHandle SpecHandle : AbilitySpecHandles)
	{
		if (SpecHandle.IsValid())
		{
			AbilitySystem->ClearAbility(SpecHandle);
		}
	}
	AbilitySpecHandles.Reset();

	// Remove the applied gameplay effects
	for (FActiveGameplayEffectHandle EffectHandle : GameplayEffectHandles)
	{
		if (EffectHandle.IsValid())
		{
			AbilitySystem->RemoveActiveGameplayEffect(EffectHandle);
		}
	}
	GameplayEffectHandles.Reset();
}

UMultiplayerShooterAbilitySet::UMultiplayerShooterAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMultiplayerShooterAbilitySet::GiveToAbilitySystem(FMultiplayerShooterAbilitySet_GrantedHandles& OutGrantedHandles,
	UAbilitySystemComponent* AbilitySystem, UObject* SourceObject) const
{
	check(AbilitySystem);

	if (!AbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}

	// Grant the gameplay abilities
	for (int32 Index = 0; Index < AbilitiesToGrant.Num(); ++Index)
	{
		const FMultiplayerShooterAbilitySet_GameplayAbility& AbilityToGrant = AbilitiesToGrant[Index];

		if (!IsValid(AbilityToGrant.AbilityClass))
		{
			UE_LOG(LogTemp, Error, TEXT("AbilitiesToGrant[%d] on ability set [%s] is not valid."), Index, *GetNameSafe(this));
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityToGrant.AbilityClass, AbilityToGrant.Level);
		AbilitySpec.SourceObject = SourceObject;
		if (AbilityToGrant.InputTag.IsValid())
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);
		}
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystem->GiveAbility(AbilitySpec);
		OutGrantedHandles.AbilitySpecHandles.Add(SpecHandle);
	}

	// Apply the gameplay effects
	for (int32 Index = 0; Index < GameplayEffectsToApply.Num(); ++Index)
	{
		const FMultiplayerShooterAbilitySet_GameplayEffect& EffectToApply = GameplayEffectsToApply[Index];

		if (!IsValid(EffectToApply.EffectClass))
		{
			UE_LOG(LogTemp, Error, TEXT("GameplayEffectsToApply[%d] on ability set [%s] is not valid"), Index, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToApply.EffectClass->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystem->ApplyGameplayEffectToSelf(
			GameplayEffect,
			EffectToApply.Level,
			AbilitySystem->MakeEffectContext()
		);

		OutGrantedHandles.GameplayEffectHandles.Add(ActiveEffectHandle);
	}
}
