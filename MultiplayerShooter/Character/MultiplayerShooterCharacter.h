#pragma once

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MultiplayerShooterCharacter.generated.h"

struct FGameplayTag;
class UMultiplayerShooterEquipmentInstance;
class UGameplayAbility;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMultiplayerShooterCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <typename T> requires std::is_base_of_v<UAbilitySystemComponent, T>
	T* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

	UAttributeSet* GetAttributeSet() const;

	template <typename T> requires(std::is_base_of_v<UAttributeSet, T>)
	T* GetAttributeSet() const
	{
		return Cast<T>(GetAttributeSet());
	}
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(Transient, BlueprintReadWrite)
	FVector LeftHandEffectorLocation;
};
