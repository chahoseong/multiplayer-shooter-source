#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitUntil.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FAbilityTaskPredicate);

UCLASS()
class MULTIPLAYERSHOOTER_API UAbilityTask_WaitUntil : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="True"))
	static UAbilityTask_WaitUntil* WaitUntil(
		UGameplayAbility* OwningAbility,
		FAbilityTaskPredicate Predicate,
		float Timeout = -1.0f);

public:
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnSignal;
	
	UPROPERTY(BlueprintAssignable)
	FGenericGameplayTaskDelegate OnTimeout;

protected:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

private:
	FAbilityTaskPredicate Predicate;
	float TimeoutRemaining = -1.0f;
	bool bHasTimeout = false;
};
