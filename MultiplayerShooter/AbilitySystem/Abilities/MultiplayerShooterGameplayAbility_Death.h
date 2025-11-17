#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooterGameplayAbility.h"
#include "MultiplayerShooterGameplayAbility_Death.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterGameplayAbility_Death : public UMultiplayerShooterGameplayAbility
{
	GENERATED_BODY()

public:
	UMultiplayerShooterGameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void StartDeath();
	void FinishDeath();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MultiplayerShooter|Death")
	bool bAutoStartDeath = true;
};
