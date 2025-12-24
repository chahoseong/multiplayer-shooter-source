#pragma once

#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "DustAnimInstance.generated.h"

class UAbilitySystemComponent;

UCLASS(Config=Game)
class DUST_API UDustAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UDustAnimInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
	
	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1.0f;
};
