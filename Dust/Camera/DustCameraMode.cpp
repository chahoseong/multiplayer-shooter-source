#include "Camera/DustCameraMode.h"
#include "Camera/DustCameraStateComponent.h"
#include "Camera/DustPlayerCameraManager.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"

/*
 * FDustCameraModeView
 */

FDustCameraModeView::FDustCameraModeView()
	: SpringArmLocation(ForceInit),
	TargetArmLength(350.0f),
	SocketOffset(ForceInit),
	TargetOffset(ForceInit),
	FieldOfView(DUST_CAMERA_DEFAULT_FOV),
	ControlRotation(ForceInit)
{
}

void FDustCameraModeView::Blend(const FDustCameraModeView& OtherView, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	
	if (OtherWeight >= 1.0f)
	{
		*this = OtherView;
		return;
	}
	
	// Spring Arm
	SpringArmLocation  = FMath::Lerp(SpringArmLocation, OtherView.SpringArmLocation, OtherWeight);
	TargetArmLength = FMath::Lerp(TargetArmLength, OtherView.TargetArmLength, OtherWeight);
	SocketOffset = FMath::Lerp(SocketOffset, OtherView.SocketOffset, OtherWeight);
	TargetOffset = FMath::Lerp(TargetOffset, OtherView.TargetOffset, OtherWeight);
	
	// Camera
	FieldOfView = FMath::Lerp(FieldOfView, OtherView.FieldOfView, OtherWeight);
	
	// Player Controller
	const FRotator DeltaControlRotation = (OtherView.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation += OtherWeight * DeltaControlRotation;
}

/*
 * UDustCameraMode
 */

UDustCameraMode::UDustCameraMode()
{
	FieldOfView = DUST_CAMERA_DEFAULT_FOV;
	ViewPitchMin = DUST_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = DUST_CAMERA_DEFAULT_PITCH_MAX;
}

void UDustCameraMode::Activate()
{
}

void UDustCameraMode::Deactivate()
{
}

void UDustCameraMode::Update(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void UDustCameraMode::UpdateView(float DeltaTime)
{
	UpdateForTarget(DeltaTime);
	UpdateCrouchOffset(DeltaTime);
	
	// Spring Arm
	View.SpringArmLocation = SpringArmLocation + CurrentCrouchOffset;
	View.TargetArmLength = TargetArmLength;
	View.SocketOffset = SocketOffset;
	View.TargetOffset = TargetOffset;
	
	// Camera
	View.FieldOfView = FieldOfView;
	
	// Player Controller
	FRotator PivotRotation = GetPivotRotation();
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);
	View.ControlRotation = PivotRotation;
}

void UDustCameraMode::UpdateForTarget(float DeltaTime)
{
	if (const ACharacter* TargetCharacter = Cast<ACharacter>(GetTargetActor()))
	{
		if (TargetCharacter->IsCrouched())
		{
			const ACharacter* CharacterDefault =  TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
			const float CrouchedHeightAdjustment = CharacterDefault->CrouchedEyeHeight - TargetCharacter->BaseEyeHeight;
			SetTargetCrouchOffset(FVector(0.0f, 0.0f, CrouchedHeightAdjustment));
			return;
		}
	}
	
	SetTargetCrouchOffset(FVector::ZeroVector);
}

void UDustCameraMode::SetTargetCrouchOffset(const FVector& NewTargetOffset)
{
	CrouchOffsetBlendAlpha = 0.0f;
	InitialCrouchOffset = CurrentCrouchOffset;
	TargetCrouchOffset = NewTargetOffset;
}

void UDustCameraMode::UpdateCrouchOffset(float DeltaTime)
{
	if (CrouchOffsetBlendAlpha < 1.0f)
	{
		CrouchOffsetBlendAlpha = FMath::Min(CrouchOffsetBlendAlpha + DeltaTime * CrouchOffsetBlendMultiplier, 1.0f);
		CurrentCrouchOffset = FMath::InterpEaseInOut(InitialCrouchOffset, TargetCrouchOffset, CrouchOffsetBlendAlpha, 1.0f);
	}
	else
	{
		CurrentCrouchOffset = TargetCrouchOffset;
		CrouchOffsetBlendAlpha = 1.0f;
	}
}

FRotator UDustCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);
	
	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		return TargetPawn->GetViewRotation();
	}
	
	return TargetActor->GetActorRotation();
}

void UDustCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.0f)
	{
		// 전체 소요 시간(BlendTime) 중에 얼마나 진행 했는지(DeltaTime)를 비율로 계산합니다.
		BlendAlpha += DeltaTime / BlendTime;
		
		// [0, 1] 사이로 정규화합니다.
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}
	
	const float ActualExponent = BlendExponent > 0.0f ? BlendExponent : 1.0f;
	
	switch (BlendFunction)
	{
	case EDustCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case EDustCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, ActualExponent);
		break;
	case EDustCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, ActualExponent);
		break;
	case EDustCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, ActualExponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), static_cast<uint8>(BlendFunction));
		break;
	}
}

UWorld* UDustCameraMode::GetWorld() const
{
	return HasAnyFlags(RF_ClassDefaultObject) ? nullptr : GetOuter()->GetWorld();
}

UDustCameraStateComponent* UDustCameraMode::GetCameraState() const
{
	return CastChecked<UDustCameraStateComponent>(GetOuter());
}

AActor* UDustCameraMode::GetTargetActor() const
{
	const UDustCameraStateComponent* CameraState = GetCameraState();
	return CameraState ? CameraState->GetTargetActor() : nullptr;
}

const FGameplayTag& UDustCameraMode::GetCameraTypeTag() const
{
	return CameraTypeTag;
}

float UDustCameraMode::GetBlendTime() const
{
	return BlendTime;
}

float UDustCameraMode::GetBlendWeight() const
{
	return BlendWeight;
}

void UDustCameraMode::SetBlendWeight(float Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);
	
	const float InvExponent = BlendExponent > 0.0f ? 1.0f / BlendExponent : 1.0f;
	
	switch (BlendFunction)
	{
	case EDustCameraModeBlendFunction::Linear:
		BlendAlpha = BlendWeight;
		break;
	case EDustCameraModeBlendFunction::EaseIn:
		BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, InvExponent);
		break;
	case EDustCameraModeBlendFunction::EaseOut:
		BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;
	case EDustCameraModeBlendFunction::EaseInOut:
		BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;
	default:
		checkf(false, TEXT("SetBlendWeight: Invalid BlendFunction [%d]\n"), static_cast<uint8>(BlendFunction));
		break;
	}
}

const FDustCameraModeView& UDustCameraMode::GetCameraModeView() const
{
	return View;
}

void UDustCameraMode::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);
	
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;
	DisplayDebugManager.SetDrawColor(FColor::White);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("      DustCameraMode: %s (%f)"), *GetName(), BlendWeight));
}
