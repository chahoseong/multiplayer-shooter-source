#include "Character/MultiplayerShooterCharacter.h"
#include "MultiplayerShooterCharacterMovementComponent.h"
#include "Player/MultiplayerShooterPlayerState.h"

AMultiplayerShooterCharacter::AMultiplayerShooterCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMultiplayerShooterCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	UMultiplayerShooterCharacterMovementComponent* MultiplayerShooterCharacterMovement =
		CastChecked<UMultiplayerShooterCharacterMovementComponent>(GetCharacterMovement());
	MultiplayerShooterCharacterMovement->GravityScale = 1.0f;
	MultiplayerShooterCharacterMovement->MaxAcceleration = 2400.0f;
	MultiplayerShooterCharacterMovement->BrakingFrictionFactor = 1.0f;
	MultiplayerShooterCharacterMovement->BrakingFriction = 6.0f;
	MultiplayerShooterCharacterMovement->GroundFriction = 8.0f;
	MultiplayerShooterCharacterMovement->BrakingDecelerationWalking = 1400.0f;
	MultiplayerShooterCharacterMovement->bUseControllerDesiredRotation = false;
	MultiplayerShooterCharacterMovement->bOrientRotationToMovement = false;
	MultiplayerShooterCharacterMovement->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MultiplayerShooterCharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = false;
	MultiplayerShooterCharacterMovement->GetNavAgentPropertiesRef().bCanCrouch = true;
	MultiplayerShooterCharacterMovement->bCanWalkOffLedgesWhenCrouching = true;
	MultiplayerShooterCharacterMovement->SetCrouchedHalfHeight(65.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

UAbilitySystemComponent* AMultiplayerShooterCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AMultiplayerShooterCharacter::GetAttributeSet() const
{
	return AttributeSet;
}
