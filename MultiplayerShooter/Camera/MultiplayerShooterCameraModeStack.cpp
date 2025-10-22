#include "Camera/MultiplayerShooterCameraModeStack.h"
#include "Camera/MultiplayerShooterCameraMode.h"

void UMultiplayerShooterCameraModeStack::ActivateStack()
{
	if (!bIsActive)
	{
		for (UMultiplayerShooterCameraMode* CameraMode : CameraModeStack)
		{
			check(CameraMode);
			CameraMode->OnActivation();
		}

		bIsActive = true;
	}
}

void UMultiplayerShooterCameraModeStack::DeactivateStack()
{
	if (bIsActive)
	{
		for (UMultiplayerShooterCameraMode* CameraMode : CameraModeStack)
		{
			check(CameraMode);
			CameraMode->OnDeactivation();
		}

		bIsActive = false;
	}
}

void UMultiplayerShooterCameraModeStack::PushCameraMode(TSubclassOf<UMultiplayerShooterCameraMode> CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	UMultiplayerShooterCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if (IsTop(CameraMode))
	{
		return;
	}

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;
	if (TryGetCameraMode(CameraMode, ExistingStackIndex, ExistingStackContribution))
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}

	const bool bShouldBlend = CameraMode->GetBlendTime() > 0.0f && StackSize > 0;
	const float BlendWeight = bShouldBlend ? ExistingStackContribution : 1.0f;
	CameraMode->SetBlendWeight(BlendWeight);

	CameraModeStack.Insert(CameraMode, 0);
	CameraModeStack.Last()->SetBlendWeight(1.0f);

	if (ExistingStackIndex == INDEX_NONE)
	{
		CameraMode->OnActivation();
	}
}

bool UMultiplayerShooterCameraModeStack::IsTop(const UMultiplayerShooterCameraMode* CameraMode) const
{
	return CameraModeStack.Num() > 0 && Peek() == CameraMode;
}

bool UMultiplayerShooterCameraModeStack::TryGetCameraMode(UMultiplayerShooterCameraMode* CameraMode, int32& OutIndex,
	float& OutStackContribution) const
{
	OutIndex = INDEX_NONE;
	OutStackContribution = 1.0f;

	for (int32 StackIndex = 0; StackIndex < CameraModeStack.Num(); ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			OutIndex = StackIndex;
			OutStackContribution *= CameraMode->GetBlendWeight();
			return true;
		}

		OutStackContribution *= 1.0f - CameraModeStack[StackIndex]->GetBlendWeight();
	}

	OutStackContribution = 0.0f;
	
	return false;
}

bool UMultiplayerShooterCameraModeStack::EvaluateStack(float DeltaTime,
                                                       FMultiplayerShooterCameraModeView& OutCameraModeView)
{
	if (!bIsActive)
	{
		return false;
	}

	UpdateStack(DeltaTime);
	BlendStack(OutCameraModeView);

	return true;
}

bool UMultiplayerShooterCameraModeStack::IsActive() const
{
	return bIsActive;
}

void UMultiplayerShooterCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UMultiplayerShooterCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		if (CameraMode->GetBlendWeight() >= 1.0f)
		{
			RemoveIndex = StackIndex + 1;
			RemoveCount = StackSize - RemoveIndex;
			break;
		}
	}

	if (RemoveCount > 0)
	{
		for (int32 StackIndex = RemoveIndex; StackIndex < StackSize; ++StackIndex)
		{
			UMultiplayerShooterCameraMode* CameraMode = CameraModeStack[StackIndex];
			check(CameraMode);
			CameraMode->OnDeactivation();
		}

		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UMultiplayerShooterCameraModeStack::BlendStack(FMultiplayerShooterCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	const UMultiplayerShooterCameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	OutCameraModeView = CameraMode->GetCameraModeView();

	for (int32 StackIndex = StackSize - 2; StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);
		OutCameraModeView.Blend(CameraMode->GetCameraModeView(), CameraMode->GetBlendWeight());
	}
}

UMultiplayerShooterCameraMode* UMultiplayerShooterCameraModeStack::GetCameraModeInstance(TSubclassOf<UMultiplayerShooterCameraMode> CameraModeClass) const
{
	check(CameraModeClass);

	for (UMultiplayerShooterCameraMode* CameraMode : CameraModeInstances)
	{
		if (IsValid(CameraMode) && CameraMode->GetClass() == CameraModeClass)
		{
			return CameraMode;
		}
	}

	UMultiplayerShooterCameraMode* NewCameraMode = NewObject<UMultiplayerShooterCameraMode>(
		GetOuter(),
		CameraModeClass,
		NAME_None,
		RF_NoFlags
	);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

UMultiplayerShooterCameraMode* UMultiplayerShooterCameraModeStack::Peek() const
{
	return CameraModeStack.Num() > 0 ? CameraModeStack[0] : nullptr;
}
