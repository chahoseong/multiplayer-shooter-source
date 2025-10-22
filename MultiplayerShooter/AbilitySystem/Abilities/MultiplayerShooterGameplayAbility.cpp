#include "AbilitySystem/Abilities/MultiplayerShooterGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Camera/MultiplayerShooterCameraMode.h"
#include "Camera/MultiplayerShooterCameraStateComponent.h"
#include "Character/MultiplayerShooterPlayerCharacter.h"

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

AController* UMultiplayerShooterGameplayAbility::GetTypedControllerFromActorInfo(
	TSubclassOf<AController> ControllerType) const
{
	if (UClass* ActualControllerType = ControllerType)
	{
		if (AController* Controller = GetControllerFromActorInfo();
			Controller->IsA(ActualControllerType))
		{
			return Controller;
		}
	}

	return nullptr;
}

void UMultiplayerShooterGameplayAbility::SetCameraMode(TSubclassOf<UMultiplayerShooterCameraMode> CameraMode)
{
	AMultiplayerShooterPlayerCharacter* PlayerCharacter =
		Cast<AMultiplayerShooterPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (PlayerCharacter)
	{
		PlayerCharacter->SetAbilityCameraMode(CameraMode, CurrentSpecHandle);
		ActiveCameraMode = CameraMode;
	}
}

void UMultiplayerShooterGameplayAbility::ClearCameraMode()
{
	if (ActiveCameraMode)
	{
		AMultiplayerShooterPlayerCharacter* PlayerCharacter =
			Cast<AMultiplayerShooterPlayerCharacter>(GetAvatarActorFromActorInfo());
		if (PlayerCharacter)
		{
			PlayerCharacter->ClearAbilityCameraMode(CurrentSpecHandle);
		}
	}
}

void UMultiplayerShooterGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                       const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	TryActivateAbilityOnGiven(ActorInfo, Spec);
}

void UMultiplayerShooterGameplayAbility::TryActivateAbilityOnGiven(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	// Try to activate if activation policy is on granted
	if (ActorInfo && !Spec.IsActive() && ActivationPolicy == EMultiplayerShooterAbilityActivationPolicy::OnGranted)
	{
		UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
		const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (AbilitySystem && AvatarActor && !AvatarActor->GetTearOff() && AvatarActor->GetLifeSpan() <= 0.0f)
		{
			const bool bIsLocalExecution = NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted || EGameplayAbilityNetExecutionPolicy::LocalOnly;
			const bool bIsServerExecution = NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly || EGameplayAbilityNetExecutionPolicy::ServerInitiated;

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				AbilitySystem->TryActivateAbility(Spec.Handle);
			}
		}
	}
}
