#include "Player/MultiplayerShooterPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AMultiplayerShooterPlayerController::AMultiplayerShooterPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMultiplayerShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(
		MoveAction,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::Input_Move
	);
	EnhancedInputComponent->BindAction(
		LookAction,
		ETriggerEvent::Triggered,
		this,
		&ThisClass::Input_Look
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
