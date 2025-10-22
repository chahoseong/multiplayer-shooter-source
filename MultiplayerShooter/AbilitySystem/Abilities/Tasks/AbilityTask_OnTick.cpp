#include "AbilitySystem/Abilities/Tasks/AbilityTask_OnTick.h"

UAbilityTask_OnTick* UAbilityTask_OnTick::OnTick(UGameplayAbility* OwningAbility)
{
	UAbilityTask_OnTick* Task = NewAbilityTask<UAbilityTask_OnTick>(OwningAbility);
	Task->bTickingTask = 1;
	return Task;
}

void UAbilityTask_OnTick::TickTask(float DeltaTime)
{
	if (!Ability || !ShouldBroadcastAbilityTaskDelegates())
	{
		EndTask();
		return;
	}

	OnSignal.Broadcast(DeltaTime);
}
