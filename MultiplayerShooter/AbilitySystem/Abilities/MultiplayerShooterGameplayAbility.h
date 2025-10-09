#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MultiplayerShooterGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EMultiplayerShooterAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnGranted,
	Manually,
};

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMultiplayerShooterGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	EMultiplayerShooterAbilityActivationPolicy GetActivationPolicy() const;

	UFUNCTION(BlueprintPure, Category="MultiplayerShooter|Ability")
	AController* GetControllerFromActorInfo() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MultiplayerShooter|Ability Activation")
	EMultiplayerShooterAbilityActivationPolicy ActivationPolicy;
};
