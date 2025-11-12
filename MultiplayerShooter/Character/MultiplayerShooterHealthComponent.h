#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/ActorComponent.h"
#include "MultiplayerShooterHealthComponent.generated.h"

struct FOnAttributeChangeData;
class UMultiplayerShooterHealthSet;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EMultiplayerShooterDeathState : uint8
{
	NotDead,
	DeathStarted,
	DeathFinished,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMultiplayerShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	static UMultiplayerShooterHealthComponent* FindHealthComponent(const AActor* Actor);
	
public:
	UMultiplayerShooterHealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);
	void ReleaseFromAbilitySystem(UAbilitySystemComponent* AbilitySystem);

	virtual void StartDeath();
	virtual void FinishDeath();
	
	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	EMultiplayerShooterDeathState GetDeathState() const;

protected:
	UFUNCTION()
	void OnRep_DeathState(EMultiplayerShooterDeathState OldDeathState);
	
protected:
	UPROPERTY()
	TObjectPtr<const UMultiplayerShooterHealthSet> HealthSet;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_DeathState)
	EMultiplayerShooterDeathState DeathState;
};
