#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "MultiplayerShooterDamageExecution.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UMultiplayerShooterDamageExecution();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
