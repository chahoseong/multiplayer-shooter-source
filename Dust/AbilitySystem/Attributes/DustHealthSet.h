#pragma once

#include "DustAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DustHealthSet.generated.h"

UCLASS(BlueprintType)
class DUST_API UDustHealthSet : public UDustAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(UDustHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UDustHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UDustHealthSet, Damage);
	
	mutable FDustAttributeEvent OnHealthChanged;
	mutable FDustAttributeEvent OnMaxHealthChanged;
	mutable FDustAttributeEvent OnOutOfHealth;
	
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	float ClampAttribute(const FGameplayAttribute& Attribute, float Value) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Dust|Health")
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Dust|Health")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, Category="Dust|Health")
	FGameplayAttributeData Damage;
	
	float HealthBeforeAttributeChange = 0.0f;
	float MaxHealthBeforeAttributeChange = 0.0f;
	
	bool bOutOfHealth;
};
