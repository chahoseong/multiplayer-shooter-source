#include "Player/MultiplayerShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MultiplayerShooterGameplayTags.h"
#include "AbilitySystem/MultiplayerShooterAbilitySystemComponent.h"
#include "Character/MultiplayerShooterCharacter.h"
#include "Input/MultiplayerShooterInputComponent.h"

AMultiplayerShooterPlayerController::AMultiplayerShooterPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMultiplayerShooterPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	UMultiplayerShooterAbilitySystemComponent* AbilitySystem =
		GetMultiplayerShooterAbilitySystemComponent();
	if (AbilitySystem)
	{
		AbilitySystem->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AMultiplayerShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UMultiplayerShooterInputComponent* MultiplayerShooterInputComponent =
		CastChecked<UMultiplayerShooterInputComponent>(InputComponent);
	
	MultiplayerShooterInputComponent->BindNativeAction(
		InputConfig,
		MultiplayerShooterGameplayTags::Input_Action_Move,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::Input_Move,
		false
	);
	MultiplayerShooterInputComponent->BindNativeAction(
		InputConfig,
		MultiplayerShooterGameplayTags::Input_Action_Look,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::Input_Look,
		false
	);

	MultiplayerShooterInputComponent->BindAbilityActions(
		InputConfig,
		this,
		&ThisClass::Input_AbilityInputPressed,
		&ThisClass::Input_AbilityInputReleased
	);
}

void AMultiplayerShooterPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementInput = InputActionValue.Get<FVector2D>();
	const FRotator TargetRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	if (APawn* ControlledPawn = GetPawn())
	{
		if (MovementInput.X != 0.0f)
		{
			const FVector RightDirection = TargetRotation.RotateVector(FVector::RightVector);
			ControlledPawn->AddMovementInput(RightDirection, MovementInput.X);
		}

		if (MovementInput.Y != 0.0f)
		{
			const FVector ForwardDirection = TargetRotation.RotateVector(FVector::ForwardVector);
			ControlledPawn->AddMovementInput(ForwardDirection, MovementInput.Y);
		}
	}
}

void AMultiplayerShooterPlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookInput = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		if (LookInput.X != 0.0f)
		{
			ControlledPawn->AddControllerYawInput(LookInput.X);
		}

		if (LookInput.Y != 0.0f)
		{
			ControlledPawn->AddControllerPitchInput(LookInput.Y);
		}
	}
}

void AMultiplayerShooterPlayerController::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	UMultiplayerShooterAbilitySystemComponent* AbilitySystem =
		GetMultiplayerShooterAbilitySystemComponent();
	if (AbilitySystem)
	{
		AbilitySystem->AbilityInputPressed(InputTag);
	}
}

void AMultiplayerShooterPlayerController::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	UMultiplayerShooterAbilitySystemComponent* AbilitySystem =
		GetMultiplayerShooterAbilitySystemComponent();
	if (AbilitySystem)
	{
		AbilitySystem->AbilityInputReleased(InputTag);
	}
}

void AMultiplayerShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		Subsystem->AddMappingContext(DefaultInputMapping, 0);
	}
}

UMultiplayerShooterAbilitySystemComponent* AMultiplayerShooterPlayerController::GetMultiplayerShooterAbilitySystemComponent() const
{
	const AMultiplayerShooterCharacter* MultiplayerShooterCharacter =
		GetPawn<AMultiplayerShooterCharacter>();
	return IsValid(MultiplayerShooterCharacter)
		       ? MultiplayerShooterCharacter->GetAbilitySystemComponent<UMultiplayerShooterAbilitySystemComponent>()
		       : nullptr;
}
