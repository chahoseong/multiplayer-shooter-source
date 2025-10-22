#include "Player/MultiplayerShooterPlayerController.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputSubsystems.h"
#include "MultiplayerShooterGameplayTags.h"
#include "MultiplayerShooterPlayerState.h"
#include "AbilitySystem/MultiplayerShooterAbilitySystemComponent.h"
#include "Camera/MultiplayerShooterPlayerCameraManager.h"
#include "Input/MultiplayerShooterInputComponent.h"
#include "UI/MultiplayerShooterHUD.h"
#include "UI/MultiplayerShooterOverlayController.h"

AMultiplayerShooterPlayerController::AMultiplayerShooterPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerCameraManagerClass = AMultiplayerShooterPlayerCameraManager::StaticClass();
}

void AMultiplayerShooterPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	GetHitResultFromPlayerViewPoint(AimHitResult, AimMaxRange);
	if (const AMultiplayerShooterHUD* MultiplayerShooterHUD = GetMultiplayerShooterHUD())
	{
		UMultiplayerShooterOverlayController* OverlayController =
			MultiplayerShooterHUD->GetOverlayController();
		if (OverlayController)
		{
			OverlayController->SetTarget(AimHitResult.GetActor());
		}
	}
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

void AMultiplayerShooterPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (GetWorld()->IsNetMode(NM_Client))
	{
		AMultiplayerShooterPlayerState* MultiplayerShooterPlayerState =
			GetPlayerState<AMultiplayerShooterPlayerState>();
		UMultiplayerShooterAbilitySystemComponent* AbilitySystem =
			MultiplayerShooterPlayerState->GetAbilitySystemComponent<UMultiplayerShooterAbilitySystemComponent>();
		if (MultiplayerShooterPlayerState && AbilitySystem)
		{
			AbilitySystem->RefreshAbilityActorInfo();
			AbilitySystem->TryActivateAbilityOnGiven();
		}
	}
}

void AMultiplayerShooterPlayerController::GetHitResultFromPlayerViewPoint(FHitResult& OutHit, float MaxRange) const
{
	FVector ViewStart;
	FRotator ViewRotation;
	GetPlayerViewPoint(ViewStart, ViewRotation);
	FVector ViewEnd = ViewStart + ViewRotation.Vector() * AimMaxRange;

	FCollisionQueryParams QueryParams(
	SCENE_QUERY_STAT(AMultiplayerShooterPlayerController_GetHitResultFromPlayerViewPoint),
		false,
		GetPawn()
	);
	
	GetWorld()->LineTraceSingleByChannel(
		OutHit,
		ViewStart,
		ViewEnd,
		ECC_Visibility,
		QueryParams
	);

	if (!OutHit.bBlockingHit)
	{
		OutHit.ImpactPoint = ViewEnd;
	}
}

const FHitResult& AMultiplayerShooterPlayerController::GetAimHitResult() const
{
	return AimHitResult;
}

AMultiplayerShooterHUD* AMultiplayerShooterPlayerController::GetMultiplayerShooterHUD() const
{
	return GetHUD<AMultiplayerShooterHUD>();
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

		AMultiplayerShooterHUD* MultiplayerShooterHUD = GetHUD<AMultiplayerShooterHUD>();
		MultiplayerShooterHUD->InitOverlay();
	}
}

UMultiplayerShooterAbilitySystemComponent* AMultiplayerShooterPlayerController::GetMultiplayerShooterAbilitySystemComponent() const
{
	UAbilitySystemComponent* AbilitySystem =
		UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
	return Cast<UMultiplayerShooterAbilitySystemComponent>(AbilitySystem);
}
