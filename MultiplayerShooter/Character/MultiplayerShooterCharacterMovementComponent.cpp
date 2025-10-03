#include "Character/MultiplayerShooterCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

namespace MultiplayerShooterCharacter
{
	constexpr float GroundTraceDistance = 10000.0f;
}

UMultiplayerShooterCharacterMovementComponent::UMultiplayerShooterCharacterMovementComponent(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMultiplayerShooterCharacterMovementComponent::SimulateMovement(float DeltaTime)
{
	if (bHasReplicatedAcceleration)
	{
		const FVector OriginalAcceleration = Acceleration;
		Super::SimulateMovement(DeltaTime);
		Acceleration = OriginalAcceleration;
	}
	else
	{
		Super::SimulateMovement(DeltaTime);
	}

}

bool UMultiplayerShooterCharacterMovementComponent::CanAttemptJump() const
{
	// // Same as UCharacterMovementComponent's implementation but without the crouch check
	return IsJumpAllowed() && (IsMovingOnGround() || IsFalling());
}

FRotator UMultiplayerShooterCharacterMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	return Super::GetDeltaRotation(DeltaTime);
}

float UMultiplayerShooterCharacterMovementComponent::GetMaxSpeed() const
{
	return Super::GetMaxSpeed();
}

const FMultiplayerShooterCharacterGroundInfo& UMultiplayerShooterCharacterMovementComponent::GetGroundInfo() const
{
	if (!CharacterOwner || GFrameCounter == CachedGroundInfo.LastUpdateFrame)
	{
		return CachedGroundInfo;
	}

	if (MovementMode == MOVE_Walking)
	{
		CachedGroundInfo.GroundHitResult = CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* Capsule = CharacterOwner->GetCapsuleComponent();
		check(Capsule);

		const float CapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn;
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, TraceStart.Z - MultiplayerShooterCharacter::GroundTraceDistance - CapsuleHalfHeight);

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(MultiplayerShooterCharacterMovementComponent_GetGroundInfo));
		FCollisionResponseParams ResponseParams;
		InitCollisionParams(QueryParams, ResponseParams);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			CollisionChannel,
			QueryParams,
			ResponseParams
		);

		CachedGroundInfo.GroundHitResult = HitResult;
		CachedGroundInfo.GroundDistance = MultiplayerShooterCharacter::GroundTraceDistance;

		if (MovementMode == MOVE_NavWalking)
		{
			CachedGroundInfo.GroundDistance = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			CachedGroundInfo.GroundDistance = FMath::Max(HitResult.Distance - CapsuleHalfHeight, 0.0f);
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;

	return CachedGroundInfo;
}

void UMultiplayerShooterCharacterMovementComponent::SetReplicatedAcceleration(const FVector& InAcceleration)
{
	bHasReplicatedAcceleration = true;
	Acceleration = InAcceleration;
}
