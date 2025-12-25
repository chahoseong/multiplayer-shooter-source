#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DustAbilitySet.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class UGameplayEffect;
class UDustGameplayAbility;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FDustAbilitySet_GameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDustGameplayAbility> Ability;
	
	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;
	
	UPROPERTY(EditDefaultsOnly, meta=(Categories="Input.Action"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FDustAbilitySet_GameplayEffect
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	
	UPROPERTY(EditDefaultsOnly)
	float Level = 1.0f;
};

USTRUCT(BlueprintType)
struct FDustAbilitySet_GrantedHandles
{
	GENERATED_BODY()
	
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	
	void TakeFromAbilitySystem(UAbilitySystemComponent* AbilitySystem);
	
protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};


UCLASS()
class DUST_API UDustAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDustAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void GiveToAbilitySystem(UAbilitySystemComponent* AbilitySystem,
		FDustAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FDustAbilitySet_GameplayAbility> GrantedGameplayAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FDustAbilitySet_GameplayEffect> GrantedGameplayEffects;
};
