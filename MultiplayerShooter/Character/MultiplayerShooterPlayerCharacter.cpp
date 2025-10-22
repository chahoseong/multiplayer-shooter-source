#include "Character/MultiplayerShooterPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterGameplayTags.h"
#include "Animation/MultiplayerShooterAnimInstance.h"
#include "Camera/MultiplayerShooterCameraMode.h"
#include "Camera/MultiplayerShooterCameraStateComponent.h"
#include "Equipment/MultiplayerShooterEquipmentManagerComponent.h"
#include "Player/MultiplayerShooterPlayerController.h"
#include "Player/MultiplayerShooterPlayerState.h"

AMultiplayerShooterPlayerCharacter::AMultiplayerShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EquipmentManagerComponent = CreateDefaultSubobject<UMultiplayerShooterEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));
	CameraStateComponent = CreateDefaultSubobject<UMultiplayerShooterCameraStateComponent>(TEXT("CameraStateComponent"));
}

void AMultiplayerShooterPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();

	if (AimingAbility)
	{
		FGameplayAbilitySpec AbilitySpec(AimingAbility);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(MultiplayerShooterGameplayTags::Input_Action_Aim);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	for (TSubclassOf EquipmentDefinition : StartupEquipments)
	{
		EquipmentManagerComponent->Equip(EquipmentDefinition);
	}
}

void AMultiplayerShooterPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		CameraStateComponent->DetermineCameraModeDelegate.BindUObject(
			this,
			&ThisClass::DetermineCameraMode
		);
	}
}

TSubclassOf<UMultiplayerShooterCameraMode> AMultiplayerShooterPlayerCharacter::DetermineCameraMode() const
{
	if (CurrentAbilityCameraMode)
	{
		return CurrentAbilityCameraMode;
	}
	
	return DefaultCameraMode;
}

void AMultiplayerShooterPlayerCharacter::SetAbilityCameraMode(TSubclassOf<UMultiplayerShooterCameraMode> CameraMode,
                                                              const FGameplayAbilitySpecHandle& OwningAbilitySpec)
{
	if (CameraMode)
	{
		CurrentAbilityCameraMode = CameraMode;
		AbilityCameraModeSource = OwningAbilitySpec;
	}

}

void AMultiplayerShooterPlayerCharacter::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningAbilitySpec)
{
	if (AbilityCameraModeSource == OwningAbilitySpec)
	{
		CurrentAbilityCameraMode = nullptr;
		AbilityCameraModeSource = FGameplayAbilitySpecHandle();
	}
}

void AMultiplayerShooterPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();
}

void AMultiplayerShooterPlayerCharacter::InitializeAbilitySystem()
{
	AMultiplayerShooterPlayerState* MultiplayerShooterPlayerState =
	GetPlayerStateChecked<AMultiplayerShooterPlayerState>();
	AbilitySystemComponent = MultiplayerShooterPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(MultiplayerShooterPlayerState, this);

	UMultiplayerShooterAnimInstance* MultiplayerShooterAnimInstance =
		Cast<UMultiplayerShooterAnimInstance>(GetMesh()->GetAnimInstance());
	if (MultiplayerShooterAnimInstance)
	{
		MultiplayerShooterAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}
}
