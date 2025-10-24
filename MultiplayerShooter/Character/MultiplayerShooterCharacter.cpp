#include "Character/MultiplayerShooterCharacter.h"
#include "MultiplayerShooterCharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

void AMultiplayerShooterCharacter::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	if (GetController())
	{
		if (HasAuthority())
		{
			AMultiplayerShooterPlayerState* MultiplayerShooterPlayerState =
				GetPlayerState<AMultiplayerShooterPlayerState>();
			MultiplayerShooterPlayerState->SetGenericTeamId(TeamID);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a character (%s) except on the authority"), *GetPathNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a possessed character (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
	}
}

FGenericTeamId AMultiplayerShooterCharacter::GetGenericTeamId() const
{
	const AMultiplayerShooterPlayerState* MultiplayerShooterPlayerState =
		GetPlayerState<AMultiplayerShooterPlayerState>();
	if (MultiplayerShooterPlayerState)
	{
		return MultiplayerShooterPlayerState->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}
