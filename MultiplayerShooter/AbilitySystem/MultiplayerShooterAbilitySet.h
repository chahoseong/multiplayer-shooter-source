#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MultiplayerShooterAbilitySet.generated.h"

class UGameplayEffect;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FMultiplayerShooterAbilitySet_GameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly, meta=(Categories="Input.Action"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FMultiplayerShooterAbilitySet_GameplayEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly)
	float Level = 1.0f;
};

USTRUCT(BlueprintType)
struct FMultiplayerShooterAbilitySet_GrantedHandles
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	void TakeFromAbilitySystem(UAbilitySystemComponent* AbilitySystem);
	bool IsEmpty() const;
};

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UMultiplayerShooterAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(FMultiplayerShooterAbilitySet_GrantedHandles& OutGrantedHandles, UAbilitySystemComponent* AbilitySystem, UObject* SourceObject = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities", meta=(TitleProperty="AbilityClass"))
	TArray<FMultiplayerShooterAbilitySet_GameplayAbility> AbilitiesToGrant;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effects", meta=(TitleProperty="EffectClass"))
	TArray<FMultiplayerShooterAbilitySet_GameplayEffect> GameplayEffectsToApply;
};
