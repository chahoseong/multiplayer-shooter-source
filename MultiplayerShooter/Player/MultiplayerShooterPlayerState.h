#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerShooterPlayerState.generated.h"

class UMultiplayerShooterAbilitySystemComponent;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMultiplayerShooterPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <typename T> requires std::is_base_of_v<UAbilitySystemComponent, UMultiplayerShooterAbilitySystemComponent>
	UMultiplayerShooterAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}
	
	UAttributeSet* GetAttributeSet() const;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet;
};
