#include "AbilitySystem/Abilities/Tasks/AbilityTask_AimTrace.h"
#include "AbilitySystemComponent.h"

UAbilityTask_AimTrace* UAbilityTask_AimTrace::AimTrace(UGameplayAbility* OwningAbility, float MaxRange)
{
	UAbilityTask_AimTrace* Task = NewAbilityTask<UAbilityTask_AimTrace>(OwningAbility);
	Task->MaxRange = MaxRange;
	return Task;
}

void UAbilityTask_AimTrace::Activate()
{
	if (const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo())
	{
		if (ActorInfo->IsLocallyControlled())
		{
			SendAimTraceHitResult();
			return;
		}

		if (AbilitySystemComponent.IsValid())
		{
			FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
			FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

			AbilitySystemComponent->AbilityTargetDataSetDelegate(
				AbilitySpecHandle,
				ActivationPredictionKey
			).AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);

			const bool bIsTargetDataSet = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
				AbilitySpecHandle,
				ActivationPredictionKey
			);
			if (!bIsTargetDataSet)
			{
				SetWaitingOnRemotePlayerData();
			}
		}
	}
}

void UAbilityTask_AimTrace::SendAimTraceHitResult() const
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

	APlayerController* PlayerController =
		Ability->GetCurrentActorInfo()->PlayerController.Get();
	check(PlayerController);

	FVector ViewStart;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewStart, ViewRotation);
	const FVector ViewDirection = ViewRotation.Vector();
	FVector ViewEnd = ViewStart + ViewDirection * MaxRange;
	
	UWorld* World = PlayerController->GetWorld();
	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(AimTrace_SendAimTraceHitResult),
		false,
		PlayerController->GetPawn()
	);
	
	FHitResult Hit;
	World->LineTraceSingleByChannel(
		Hit,
		ViewStart,
		ViewEnd,
		ECC_Visibility,
		QueryParams
	);

	if (!Hit.bBlockingHit)
	{
		Hit.ImpactPoint = ViewEnd;
	}

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(Hit));

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->ServerSetReplicatedTargetData(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey(),
			TargetDataHandle,
			FGameplayTag(),
			AbilitySystemComponent->ScopedPredictionKey
		);
	}
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}

void UAbilityTask_AimTrace::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data,
	FGameplayTag ActivationTag)
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->ConsumeClientReplicatedTargetData(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey()
		);
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(Data);
	}
}
