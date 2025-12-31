#include "AbilitySystem/Abilities/DustGameplayAbility.h"
#include "Camera/DustCameraMode.h"
#include "Character/DustPlayerCharacter.h"
#include "GameFramework/Character.h"

UDustGameplayAbility::UDustGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	
	ActivationPolicy = EDustAbilityActivationPolicy::OnInputTriggered;
}

void UDustGameplayAbility::SetCameraMode(TSubclassOf<UDustCameraMode> CameraMode)
{
	if (ADustPlayerCharacter* PlayerCharacter = Cast<ADustPlayerCharacter>(GetAvatarActorFromActorInfo()))
	{
		PlayerCharacter->SetCameraMode(CameraMode, this);
		ActiveCameraMode = CameraMode;
	}
}

void UDustGameplayAbility::ClearCameraMode()
{
	if (ActiveCameraMode)
	{
		if (ADustPlayerCharacter* PlayerCharacter = Cast<ADustPlayerCharacter>(GetAvatarActorFromActorInfo()))
		{
			PlayerCharacter->ClearCameraMode(this);
		}
		ActiveCameraMode = nullptr;
	}
}

EDustAbilityActivationPolicy UDustGameplayAbility::GetActivationPolicy() const
{
	return ActivationPolicy;
}

AController* UDustGameplayAbility::GetControllerFromActorInfo() const
{
	return CurrentActorInfo ? Cast<AController>(CurrentActorInfo->PlayerController.Get()) : nullptr;
}

AController* UDustGameplayAbility::GetTypedControllerFromActorInfo(TSubclassOf<AController> ControllerType) const
{
	if (CurrentActorInfo)
	{
		if (AController* Controller = CurrentActorInfo->PlayerController.Get())
		{
			return Controller;
		}
		
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		AController* Controller = nullptr;
		
		while (TestActor)
		{
			if (TestActor->IsA<AController>())
			{
				Controller = Cast<AController>(TestActor);
				break;
			}
			
			if (const APawn* Pawn = Cast<APawn>(TestActor))
			{
				Controller = Pawn->GetController();
				break;
			}
			
			TestActor = TestActor->GetOwner();
		}

		if (Controller && Controller->IsA(ControllerType->GetClass()))
		{
			return Controller;
		}
	}
	
	return nullptr;
}

ACharacter* UDustGameplayAbility::GetCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

ACharacter* UDustGameplayAbility::GetTypedCharacterFromActorInfo(TSubclassOf<ACharacter> CharacterType) const
{
	if (CurrentActorInfo)
	{
		if (AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get())
		{
			if (AvatarActor->IsA(CharacterType->GetClass()))
			{
				return Cast<ACharacter>(AvatarActor);
			}
		}
	}
	
	return nullptr;
}

