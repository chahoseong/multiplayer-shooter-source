#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_OnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameplayTaskTickDelegate, float, DeltaTime);

UCLASS()
class MULTIPLAYERSHOOTER_API UAbilityTask_OnTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="True"))
	static UAbilityTask_OnTick* OnTick(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FGameplayTaskTickDelegate OnSignal;

protected:
	virtual void TickTask(float DeltaTime) override;
};
