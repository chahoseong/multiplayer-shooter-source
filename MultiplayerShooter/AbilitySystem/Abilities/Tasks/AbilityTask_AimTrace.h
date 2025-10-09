#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_AimTrace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAimTraceHit, const FGameplayAbilityTargetDataHandle&, Data);

UCLASS()
class MULTIPLAYERSHOOTER_API UAbilityTask_AimTrace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePine="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="True"))
	static UAbilityTask_AimTrace* AimTrace(UGameplayAbility* OwningAbility, float MaxRange = 999999.0f);
	
	UPROPERTY(BlueprintAssignable)
	FOnAimTraceHit ValidData;
	
private:
	virtual void Activate() override;

	void SendAimTraceHitResult() const;
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag);

protected:
	UPROPERTY(BlueprintReadOnly)
	float MaxRange;
};
