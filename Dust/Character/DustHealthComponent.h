#pragma once

#include "Components/ActorComponent.h"
#include "DustHealthComponent.generated.h"

struct FGameplayEffectSpec;
class UDustHealthComponent;
class UDustHealthSet;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EDustDeathState : uint8
{
	NotDead,
	DeathStarted,
	DeathFinished
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDustHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDustHealth_AttributeChanged, UDustHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS()
class DUST_API UDustHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category="Dust|Health")
	static UDustHealthComponent* FindHealthComponent(const AActor* Actor);

public:
	UDustHealthComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category="Dust|Health")
	void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);
	
	void StartDeath();
	void FinishDeath();

	UFUNCTION(BlueprintPure, Category="Dust|Health")
	float GetHealth() const;
	
	UFUNCTION(BlueprintPure, Category="Dust|Health")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintPure, Category="Dust|Health")
	EDustDeathState GetDeathState() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category="Dust|Health", meta=(ExpandBoolAsExecs="ReturnValue"))
	bool IsDeadOrDying() const;
	
	UPROPERTY(BlueprintAssignable)
	FDustHealth_AttributeChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FDustHealth_AttributeChanged OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FDustHealth_DeathEvent OnDeathStarted;
	
	UPROPERTY(BlueprintAssignable)
	FDustHealth_DeathEvent OnDeathFinished;
	
protected:
	virtual void OnUnregister() override;

private:
	void HandleHealthChanged(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, 
		float EffectMagnitude, float OldHealth, float NewHealth);
	void HandleMaxHealthChanged(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, 
		float EffectMagnitude, float OldMaxHealth, float NewMaxHealth);
	void HandleOutOfHealth(AActor* EffectInstigator, AActor* EffectCauser, const FGameplayEffectSpec* EffectSpec, 
		float EffectMagnitude, float OldHealth, float NewHealth);
	
	UFUNCTION()
	void OnRep_DeathState(EDustDeathState OldDeathState);
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<const UDustHealthSet> HealthSet;
	
	UPROPERTY(ReplicatedUsing=OnRep_DeathState)
	EDustDeathState DeathState;
};
