#include "AbilitySystem/Abilities/MultiplayerShooterGameplayAbility.h"

UMultiplayerShooterGameplayAbility::UMultiplayerShooterGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EMultiplayerShooterAbilityActivationPolicy UMultiplayerShooterGameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}

AController* UMultiplayerShooterGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* Controller = CurrentActorInfo->PlayerController.Get())
		{
			return Controller;
		}

		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* Controller = Cast<AController>(TestActor))
			{
				return Controller;
			}

			if (APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}
