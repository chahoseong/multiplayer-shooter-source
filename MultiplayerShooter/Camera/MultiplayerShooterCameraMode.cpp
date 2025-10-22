#include "Camera/MultiplayerShooterCameraMode.h"
#include "Camera/MultiplayerShooterPlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

/*
 * FMultiplayerShooterCameraModeView
 */

FMultiplayerShooterCameraModeView::FMultiplayerShooterCameraModeView()
	: FieldOfView(MULTIPLAYERSHOOTER_DEFAULT_FOV)
{
	
}

void FMultiplayerShooterCameraModeView::Blend(const FMultiplayerShooterCameraModeView& Other, float Alpha)
{
	if (Alpha <= 0.0f)
	{
		return;
	}
	
	if (Alpha >= 1.0f)
	{
		*this = Other;
		return;
	}

	const FRotator DeltaRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation += Alpha * DeltaRotation;
	
	SocketOffset = FMath::Lerp(SocketOffset, Other.SocketOffset, Alpha);
	TargetArmLength = FMath::Lerp(TargetArmLength, Other.TargetArmLength, Alpha);
	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, Alpha);
}

/*
 * UMultiplayerShooterCameraMode
 */

UMultiplayerShooterCameraMode::UMultiplayerShooterCameraMode()
{
	FieldOfView = MULTIPLAYERSHOOTER_DEFAULT_FOV;
	ViewPitchMin = MULTIPLAYERSHOOTER_DEFAULT_PITCH_MIN;
	ViewPitchMax = MULTIPLAYERSHOOTER_DEFAULT_PITCH_MAX;
}

void UMultiplayerShooterCameraMode::OnActivation()
{
}

void UMultiplayerShooterCameraMode::OnDeactivation()
{
}

void UMultiplayerShooterCameraMode::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void UMultiplayerShooterCameraMode::UpdateView(float DeltaTime)
{
	FRotator ControlRotation = GetViewRotation();
	ControlRotation.Pitch = FMath::ClampAngle(ControlRotation.Pitch, ViewPitchMin, ViewPitchMax);
	
	CameraModeView.ControlRotation = ControlRotation;
	CameraModeView.SocketOffset = SocketOffset;
	CameraModeView.TargetArmLength = TargetArmLength;
	CameraModeView.FieldOfView = FieldOfView;
}

void UMultiplayerShooterCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.0f)
	{
		BlendAlpha += DeltaTime / BlendTime;
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = BlendExponent > 0.0f ? BlendExponent : 1.0f;

	switch (BlendFunction)
	{
	case EMultiplayerShooterCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case EMultiplayerShooterCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EMultiplayerShooterCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case EMultiplayerShooterCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), static_cast<uint8>(BlendFunction));
		break;
	}
}

FRotator UMultiplayerShooterCameraMode::GetViewRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

const FGameplayTag& UMultiplayerShooterCameraMode::GetCameraModeTag() const
{
	return CameraModeTag;
}

const FMultiplayerShooterCameraModeView& UMultiplayerShooterCameraMode::GetCameraModeView() const
{
	return CameraModeView;
}

float UMultiplayerShooterCameraMode::GetBlendTime() const
{
	return BlendTime;
}

float UMultiplayerShooterCameraMode::GetBlendWeight() const
{
	return BlendWeight;
}

void UMultiplayerShooterCameraMode::SetBlendWeight(float Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);
	
	const float InvExponent = BlendExponent > 0.0f ? 1.0f / BlendExponent : 1.0f;

	switch (BlendFunction)
	{
	case EMultiplayerShooterCameraModeBlendFunction::Linear:
		BlendAlpha = BlendWeight;
		break;
	case EMultiplayerShooterCameraModeBlendFunction::EaseIn:
		BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, InvExponent);
		break;
	case EMultiplayerShooterCameraModeBlendFunction::EaseOut:
		BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;
	case EMultiplayerShooterCameraModeBlendFunction::EaseInOut:
		BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;
	default:
		checkf(false, TEXT("SetBlendWeight: Invalid BlendFunction [%d]\n"), static_cast<uint8>(BlendFunction));
		break;
	}
}

AActor* UMultiplayerShooterCameraMode::GetTargetActor() const
{
	const UActorComponent* OwnerComponent = Cast<UActorComponent>(GetOuter());
	return OwnerComponent->GetOwner();
}
