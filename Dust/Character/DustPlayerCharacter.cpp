#include "Character/DustPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "DustGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "Expected.h"
#include "Input/DustInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
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
}

void ADustPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitializeWithAbilitySystem();
}

void ADustPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AddInputMappingContext();
	BindInputActions(PlayerInputComponent);
}

void ADustPlayerCharacter::AddInputMappingContext() const
{
	Expect(GetController<APlayerController>(),IsValid)
	.AndThen([](const APlayerController* PlayerController){
		return Expect(PlayerController->GetLocalPlayer());
	}).AndThen([](const ULocalPlayer* LocalPlayer){
		return Expect(ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer));
	}).AndThen([this](UEnhancedInputLocalPlayerSubsystem* Subsystem){
		Subsystem->AddMappingContext(InputMapping, 1);
	});
}

void ADustPlayerCharacter::BindInputActions(UInputComponent* PlayerInputComponent)
{
	Expect(Cast<UDustInputComponent>(PlayerInputComponent))
	.AndThen([this](UDustInputComponent* DustInputComponent){
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
	});
}

void ADustPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementInput = InputActionValue.Get<FVector2D>();
	const FRotator ControlRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	
	if (MovementInput.Y != 0.0f)
	{
		const FVector ForwardDirection = ControlRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementInput.Y);
	}
	
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

void ADustPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitializeWithAbilitySystem();
}

void ADustPlayerCharacter::InitializeWithAbilitySystem()
{
	if (ADustPlayerState* DustPlayerState = GetPlayerState<ADustPlayerState>())
	{
		AbilitySystemComponent = DustPlayerState->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(DustPlayerState, this);
	}
}

UAbilitySystemComponent* ADustPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
