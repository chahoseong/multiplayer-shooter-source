#include "Character/DustPlayerCharacter.h"
#include "DustGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "Input/DustInputComponent.h"
#include "InputActionValue.h"
#include "AbilitySystem/DustAbilitySystemComponent.h"
#include "Animation/DustAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Camera/DustCameraMode.h"
#include "Camera/DustCameraStateComponent.h"
#include "Equipment/DustEquipmentManagerComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/DustPlayerState.h"

ADustPlayerCharacter::ADustPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(GetRootComponent());
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	CameraStateComponent = CreateDefaultSubobject<UDustCameraStateComponent>(TEXT("CameraState"));
	
	EquipmentManagerComponent = CreateDefaultSubobject<UDustEquipmentManagerComponent>(TEXT("EquipmentManager"));
}

void ADustPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitializeCharacter();
}

TSubclassOf<UDustCameraMode> ADustPlayerCharacter::DetermineCameraMode() const
{
	return CurrentCameraMode ? CurrentCameraMode : DefaultCameraMode;
}

void ADustPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PlayerController = GetController<APlayerController>())
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	UDustInputComponent* DustInputComponent = Cast<UDustInputComponent>(PlayerInputComponent);
	check(DustInputComponent);
	
	// Bind native input actions
	// Move
	DustInputComponent->BindNativeAction(
		InputConfig,
		DustGameplayTags::Input_Action_Move,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::Input_Move,
		false
	);
	// Look
	DustInputComponent->BindNativeAction(
		InputConfig,
		DustGameplayTags::Input_Action_Look,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::Input_Look,
		false
	);
	
	// Bind ability input actions
	TArray<uint32> AbilityInputBindHandles;
	DustInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased, AbilityInputBindHandles);
}

void ADustPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementInput = InputActionValue.Get<FVector2D>();
	const FRotator ControlRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	
	// Forward
	if (MovementInput.Y != 0.0f)
	{
		const FVector ForwardDirection = ControlRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementInput.Y);
	}
	
	// Right
	if (MovementInput.X != 0.0f)
	{
		const FVector RightDirection = ControlRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementInput.X);
	}
}

void ADustPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookInput = InputActionValue.Get<FVector2D>();
	
	// Pitch
	if (LookInput.Y != 0.0f)
	{
		AddControllerPitchInput(LookInput.Y);
	}
	
	// Yaw
	if (LookInput.X != 0.0f)
	{
		AddControllerYawInput(LookInput.X);
	}
}

void ADustPlayerCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagPressed(InputTag);
	}
}

void ADustPlayerCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}

void ADustPlayerCharacter::SetCameraMode(TSubclassOf<UDustCameraMode> CameraMode, UObject* SourceObject)
{
	if (CameraMode)
	{
		CurrentCameraMode = CameraMode;
		CameraModeSource = SourceObject;
	}
}

void ADustPlayerCharacter::ClearCameraMode(UObject* SourceObject)
{
	if (CameraModeSource.IsValid() && CameraModeSource == SourceObject)
	{
		CurrentCameraMode = nullptr;
		CameraModeSource = nullptr;
	}
}

void ADustPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitializeCharacter();
}

void ADustPlayerCharacter::InitializeCharacter()
{
	// Camera
	InitializeCameraState();
	
	// Ability
	InitializeWithAbilitySystem();
	
	// Animation
	if (UDustAnimInstance* DustAnimInstance = Cast<UDustAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		DustAnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
	}
	
	if (HasAuthority())
	{
		// Equipment
		for (TSubclassOf Equipment : StartupEquipments)
		{
			EquipmentManagerComponent->Equip(Equipment);
		}
	}
}

void ADustPlayerCharacter::InitializeWithAbilitySystem()
{
	if (ADustPlayerState* DustPlayerState = GetPlayerState<ADustPlayerState>())
	{
		AbilitySystemComponent = DustPlayerState->GetDustAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(DustPlayerState, this);
		
		HealthSet = DustPlayerState->GetHealthSet();
	}
}

void ADustPlayerCharacter::InitializeCameraState()
{
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

UAbilitySystemComponent* ADustPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
