#include "Weapons/MultiplayerShooterRangedWeaponInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "MultiplayerShooterGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

void UMultiplayerShooterRangedWeaponInstance::Tick(float DeltaTime)
{
	if (const APawn* Pawn = GetPawn())
	{
		UpdateMovingMultiplier(Pawn, DeltaTime);
		UpdateJumpingOrFallingMultiplier(Pawn, DeltaTime);
		UpdateAimingMultiplier(Pawn, DeltaTime);
		UpdateShootingMultiplier(DeltaTime);
		CurrentSpreadAngleMultiplier =
			MovingMultiplier + JumpingOrFallingMultiplier + AimingMultiplier + ShootingMultiplier;
	}
}

void UMultiplayerShooterRangedWeaponInstance::OnEquipped()
{
	Super::OnEquipped();

	bTickable = true;
}

void UMultiplayerShooterRangedWeaponInstance::OnUnequipped()
{
	bTickable = false;
	
	Super::OnUnequipped();
}

void UMultiplayerShooterRangedWeaponInstance::UpdateMovingMultiplier(const APawn* Pawn, float DeltaTime)
{
	const UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(
		Pawn->GetMovementComponent());
	const FVector2D SpeedRange(0.0f, MovementComponent->MaxWalkSpeed);
	const FVector2D MultiplierRange(0.0f, SpreadAngleMultiplier_Moving);

	FVector CurrentVelocity = Pawn->GetVelocity();
	CurrentVelocity.Z = 0.0f;

	const float TargetMultiplier = FMath::GetMappedRangeValueClamped(
		SpeedRange,
		MultiplierRange,
		CurrentVelocity.Size()
	);

	MovingMultiplier = FMath::FInterpTo(
		MovingMultiplier,
		TargetMultiplier,
		DeltaTime,
		TransitionRate_Moving
	);
}

void UMultiplayerShooterRangedWeaponInstance::UpdateJumpingOrFallingMultiplier(const APawn* Pawn, float DeltaTime)
{
	const UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(
		Pawn->GetMovementComponent());

	if (MovementComponent->IsFalling())
	{
		JumpingOrFallingMultiplier = FMath::FInterpTo(
			JumpingOrFallingMultiplier,
			SpreadAngleMultiplier_JumpingOrFalling,
			DeltaTime,
			TransitionRate_JumpingOrFalling
		);
	}
	else
	{
		JumpingOrFallingMultiplier = FMath::FInterpTo(
			JumpingOrFallingMultiplier,
			0.0f,
			DeltaTime,
			TransitionRate_JumpingOrFalling
		);
	}
}

void UMultiplayerShooterRangedWeaponInstance::UpdateAimingMultiplier(const APawn* Pawn, float DeltaTime)
{
	UAbilitySystemComponent* AbilitySystem =
		UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
	if (AbilitySystem)
	{
		if (AbilitySystem->HasMatchingGameplayTag(MultiplayerShooterGameplayTags::State_Aiming))
		{
			AimingMultiplier = FMath::FInterpTo(
				AimingMultiplier,
				-SpreadAngleMultiplier_Aiming,
				DeltaTime,
				TransitionRate_Aiming
			);
		}
		else
		{
			AimingMultiplier = FMath::FInterpTo(
			AimingMultiplier,
			0,
				DeltaTime,
				TransitionRate_Aiming
			);
		}
	}
	else
	{
		AimingMultiplier = 0.0f;
	}
}

void UMultiplayerShooterRangedWeaponInstance::UpdateShootingMultiplier(float DeltaTime)
{
	ShootingMultiplier = FMath::FInterpTo(
		ShootingMultiplier,
		0.0f,
		DeltaTime,
		0.2f
	);
}

float UMultiplayerShooterRangedWeaponInstance::GetSpreadAngleMultiplier() const
{
	return CurrentSpreadAngleMultiplier;
}

bool UMultiplayerShooterRangedWeaponInstance::IsTickable() const
{
	return bTickable;
}

bool UMultiplayerShooterRangedWeaponInstance::IsTickableInEditor() const
{
	return false;
}

bool UMultiplayerShooterRangedWeaponInstance::IsTickableWhenPaused() const
{
	return false;
}

TStatId UMultiplayerShooterRangedWeaponInstance::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(
		UMultiplayerShooterRangedWeaponInstance,
		STATGROUP_Tickables
	);
}

UWorld* UMultiplayerShooterRangedWeaponInstance::GetWorld() const
{
	const APawn* OwningPawn = GetPawn();
	return OwningPawn ? OwningPawn->GetWorld() : nullptr;
}

float UMultiplayerShooterRangedWeaponInstance::GetBaseSpreadAngle() const
{
	return BaseSpreadAngle;
}
