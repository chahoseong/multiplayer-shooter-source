#include "AbilitySystem/Abilities/Tasks/AbilityTask_WaitUntil.h"

UAbilityTask_WaitUntil* UAbilityTask_WaitUntil::WaitUntil(
	UGameplayAbility* OwningAbility,
	FAbilityTaskPredicate Predicate,
	float Timeout)
{
	UAbilityTask_WaitUntil* Task = NewAbilityTask<UAbilityTask_WaitUntil>(OwningAbility);
	Task->Predicate = Predicate;
	Task->TimeoutRemaining = Timeout;
	Task->bTickingTask = 1;
	return Task;
}

void UAbilityTask_WaitUntil::Activate()
{
	Super::Activate();
	bHasTimeout = TimeoutRemaining >= 0.0f;
	ReadyForActivation();
}

void UAbilityTask_WaitUntil::TickTask(float DeltaTime)
{
	if (!Ability || !ShouldBroadcastAbilityTaskDelegates())
	{
		EndTask();
		return;
	}

	if (!Predicate.IsBound())
	{
		EndTask();
		return;
	}

	if (Predicate.Execute())
	{
		OnSignal.Broadcast();
		EndTask();
	}
	else if (bHasTimeout)
	{
		TimeoutRemaining -= DeltaTime;
		if (TimeoutRemaining <= 0.0f)
		{
			OnTimeout.Broadcast();
			EndTask();
		}
	}
}
