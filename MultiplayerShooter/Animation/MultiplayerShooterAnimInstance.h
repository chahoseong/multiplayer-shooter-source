#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "MultiplayerShooterAnimInstance.generated.h"

class UAbilitySystemComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMultiplayerShooterAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
	
	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1.0f;
};
