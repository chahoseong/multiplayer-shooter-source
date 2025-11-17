#pragma once

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "MultiplayerShooterCharacter.generated.h"

class UMultiplayerShooterHealthComponent;
class UMultiplayerShooterCombatSet;
class UMultiplayerShooterAbilitySet;
class UGameplayEffect;
class UMultiplayerShooterHealthSet;
struct FGameplayTag;
class UMultiplayerShooterEquipmentInstance;
class UGameplayAbility;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMultiplayerShooterCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void OnRep_PlayerState() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <typename T> requires std::is_base_of_v<UAbilitySystemComponent, T>
	T* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

	UMultiplayerShooterHealthSet* GetHealthSet() const;
	
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void InitializeAbilitySystem();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectToApply, float Level = 1.0f) const;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UMultiplayerShooterHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<UMultiplayerShooterCombatSet> CombatSet;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMultiplayerShooterHealthComponent> HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TObjectPtr<UMultiplayerShooterAbilitySet> StartupAbilitySet;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> InitialMaxHealthAttribute;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> InitialHealthAttribute;

	UPROPERTY(Transient, BlueprintReadWrite)
	FVector LeftHandEffectorLocation;

	UPROPERTY(Replicated)
	FGenericTeamId MyTeam;
};
