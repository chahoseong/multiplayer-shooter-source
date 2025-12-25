#include "AbilitySystem/DustAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "Abilities/DustGameplayAbility.h"
#include "DustLogChannels.h"

/*
 * UDustAbilitySet
 */

UDustAbilitySet::UDustAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDustAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystem,
	FDustAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(AbilitySystem);
	
	if (!AbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}
	
	// Grant the gameplay abilities
	for (int32 AbilityIndex = 0; AbilityIndex  < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FDustAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		
		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogDustAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."),
				AbilityIndex, *GetNameSafe(this));
			continue;
		}
		
		FGameplayAbilitySpec AbilitySpec(AbilityToGrant.Ability, AbilityToGrant.Level);
		AbilitySpec.SourceObject = SourceObject;
		if (AbilityToGrant.InputTag.IsValid())
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);
		}
		
		const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystem->GiveAbility(AbilitySpec);
		
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
	
	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FDustAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];
		
		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogDustAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"),
				EffectIndex, *GetNameSafe(this));
			continue;
		}
		
		const UGameplayEffect* EffectDefault = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystem->ApplyGameplayEffectToSelf(
			EffectDefault, EffectToGrant.Level, AbilitySystem->MakeEffectContext());
		
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}

/*
 * FDustAbilitySet_GrantedHandles
 */

void FDustAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FDustAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FDustAbilitySet_GrantedHandles::TakeFromAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);
	
	if (!AbilitySystem->IsOwnerActorAuthoritative())
	{
		return;
	}
	
	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->ClearAbility(Handle);
		}
	}
	
	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystem->RemoveActiveGameplayEffect(Handle);
		}
	}
	
	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
}