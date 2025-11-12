#include "Character/MultiplayerShooterPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterHealthComponent.h"
#include "Animation/MultiplayerShooterAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Camera/MultiplayerShooterCameraMode.h"
#include "Camera/MultiplayerShooterCameraStateComponent.h"
#include "Equipment/MultiplayerShooterEquipmentManagerComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/MultiplayerShooterPlayerState.h"

AMultiplayerShooterPlayerCharacter::AMultiplayerShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	EquipmentManagerComponent = CreateDefaultSubobject<UMultiplayerShooterEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));
	CameraStateComponent = CreateDefaultSubobject<UMultiplayerShooterCameraStateComponent>(TEXT("CameraStateComponent"));
}

void AMultiplayerShooterPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsLocallyControlled())
	{
		CameraStateComponent->DetermineCameraModeDelegate.BindUObject(
			this,
			&ThisClass::DetermineCameraMode
		);
		CameraStateComponent->Activate(true);
	}
	else
	{
		CameraStateComponent->Deactivate();
	}
	
	for (TSubclassOf EquipmentDefinition : StartupEquipments)
	{
		EquipmentManagerComponent->Equip(EquipmentDefinition);
	}
}

void AMultiplayerShooterPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocallyControlled())
	{
		const float DistanceToCamera =
			(CameraComponent->GetComponentLocation() - GetActorLocation()).Size();
		const bool bIsHidden = DistanceToCamera < HideDistanceThreshold;
		GetMesh()->SetVisibility(!bIsHidden);
		EquipmentManagerComponent->SetEquipmentsVisibility(!bIsHidden);
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

void AMultiplayerShooterPlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	if (IsLocallyControlled())
	{
		CameraStateComponent->DetermineCameraModeDelegate.BindUObject(
			this,
			&ThisClass::DetermineCameraMode
		);
		CameraStateComponent->Activate(true);
	}
	else
	{
		CameraStateComponent->Deactivate();
	}
}

void AMultiplayerShooterPlayerCharacter::InitializeAbilitySystem()
{
	AMultiplayerShooterPlayerState* MultiplayerShooterPlayerState =
		GetPlayerStateChecked<AMultiplayerShooterPlayerState>();

	// Ability System
	AbilitySystemComponent = MultiplayerShooterPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(MultiplayerShooterPlayerState, this);

	// Attribute Sets
	HealthSet = MultiplayerShooterPlayerState->GetHealthSet();
	CombatSet = MultiplayerShooterPlayerState->GetCombatSet();

	// Animation
	UMultiplayerShooterAnimInstance* MultiplayerShooterAnimInstance =
		Cast<UMultiplayerShooterAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(MultiplayerShooterAnimInstance))
	{
		MultiplayerShooterAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}

	// Health
	if (HealthComponent)
	{
		HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	}

	// Ability Set
	if (!MultiplayerShooterPlayerState->HasStartupAbilities())
	{
		MultiplayerShooterPlayerState->GiveStartupAbilities(StartupAbilitySet);
	}
}
