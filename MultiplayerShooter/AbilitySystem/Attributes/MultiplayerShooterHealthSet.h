#pragma once

#include "AbilitySystem/Attributes/MultiplayerShooterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterHealthSet.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerShooterOutOfHealthEvent, const FGameplayEffectSpec*);

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterHealthSet : public UMultiplayerShooterAttributeSet
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(UMultiplayerShooterHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UMultiplayerShooterHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UMultiplayerShooterHealthSet, Damage);
	
	mutable FMultiplayerShooterOutOfHealthEvent OnOutOfHealth;
	
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override; 
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

private:
	float ClampAttribute(const FGameplayAttribute& Attribute, float NewValue) const;
	
protected:
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY()
	FGameplayAttributeData Damage;

private:
	float HealthBeforeAttributeChange = 0.0f;
	float MaxHealthBeforeAttributeChange = 0.0f;
	
	bool bOutOfHealth = false;
};
