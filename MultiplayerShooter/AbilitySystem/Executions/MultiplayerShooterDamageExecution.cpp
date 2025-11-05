#include "AbilitySystem/Executions/MultiplayerShooterDamageExecution.h"
#include "AbilitySystem/Attributes/MultiplayerShooterCombatSet.h"
#include "AbilitySystem/Attributes/MultiplayerShooterHealthSet.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;

	FDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(
			UMultiplayerShooterCombatSet::GetAttackDamageAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			true
		);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UMultiplayerShooterDamageExecution::UMultiplayerShooterDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void UMultiplayerShooterDamageExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().BaseDamageDef,
		EvaluateParams,
		BaseDamage
	);

	float TotalDamageMagnitude = BaseDamage;
	TotalDamageMagnitude = FMath::Max(TotalDamageMagnitude, 0.0f);
	
	const FGameplayModifierEvaluatedData EvaluatedDamage(
		UMultiplayerShooterHealthSet::GetDamageAttribute(),
		EGameplayModOp::Additive,
		TotalDamageMagnitude
	);
	OutExecutionOutput.AddOutputModifier(EvaluatedDamage);
#endif
}
