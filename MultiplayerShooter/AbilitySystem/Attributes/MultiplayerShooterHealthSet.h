#pragma once

#include "AbilitySystem/Attributes/MultiplayerShooterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterHealthSet.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterHealthSet : public UMultiplayerShooterAttributeSet
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(UMultiplayerShooterHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UMultiplayerShooterHealthSet, MaxHealth);

protected:
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
protected:
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;

	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
};
