#include "Camera/DustCameraModeStack.h"
#include "Camera/DustCameraMode.h"
#include "Engine/Canvas.h"

UDustCameraModeStack::UDustCameraModeStack()
{
}

void UDustCameraModeStack::ActivateStack()
{
	if (!bIsActive)
	{
		bIsActive = true;
		
		for (UDustCameraMode* CameraMode : CameraModeStack)
		{
			check(CameraMode);
			CameraMode->Activate();
		}
	}
}

void UDustCameraModeStack::DeactivateStack()
{
	if (bIsActive)
	{
		bIsActive = false;
		
		for (UDustCameraMode* CameraMode : CameraModeStack)
		{
			check(CameraMode);
			CameraMode->Deactivate();
		}
	}
}

void UDustCameraModeStack::PushCameraMode(TSubclassOf<UDustCameraMode> CameraModeType)
{
	if (!CameraModeType)
	{
		return;
	}
	
	UDustCameraMode* CameraMode = GetCameraModeInstance(CameraModeType);
	check(CameraMode);
	
	int32 StackSize = CameraModeStack.Num();
	
	if (StackSize > 0 && CameraModeStack[0] == CameraMode)
	{
		return;
	}
	
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;
	
	// 추가하려는 카메라 모드가 이미 스택에 있다면, 찾아서 스택 가장 위로 이동합니다.
	// 스택의 가장 위로 이동하기 위해 해당 카메라 모드가 기존에 기여하고 있던
	// 가여도를 계산합니다.
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->GetBlendWeight();
			break;
		}
		
		ExistingStackContribution *= 1.0f - CameraModeStack[StackIndex]->GetBlendWeight();
	}
	
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		--StackSize;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}
	
	const bool bShouldBlend = CameraMode->GetBlendTime() > 0.0f && StackSize > 0;
	const float BlendWeight = bShouldBlend ? ExistingStackContribution : 1.0f;
	
	CameraMode->SetBlendWeight(BlendWeight);
	
	// 스택 가장 최상단으로 이동합니다.
	CameraModeStack.Insert(CameraMode, 0);
	
	// 스택에 가장 마지막에 있는 카메라 모드는 반드시 가중치를 1로 설정해야 합니다.
	// 카메라 모드를 블렌딩할 때, 스택의 최하단에 있는 카메라 모드를 기준으로 합니다.
	CameraModeStack.Last()->SetBlendWeight(1.0f);
	
	if (ExistingStackIndex == INDEX_NONE)
	{
		CameraMode->Activate();
	}
}

UDustCameraMode* UDustCameraModeStack::GetCameraModeInstance(TSubclassOf<UDustCameraMode> CameraModeType) const
{
	check(CameraModeType);
	
	for (UDustCameraMode* CameraMode : CameraModeInstances)
	{
		if (CameraMode && CameraMode->GetClass() == CameraModeType)
		{
			return CameraMode;
		}
	}
	
	UDustCameraMode* NewCameraMode = NewObject<UDustCameraMode>(GetOuter(), CameraModeType, NAME_None, RF_NoFlags);
	check(NewCameraMode);
	
	CameraModeInstances.Add(NewCameraMode);
	
	return NewCameraMode;
}

FDustCameraModeView UDustCameraModeStack::EvaluateStack(float DeltaTime)
{
	if (!bIsActive)
	{
		return FDustCameraModeView{};
	}
	UpdateStack(DeltaTime);
	return BlendStack();
}

void UDustCameraModeStack::UpdateStack(float DeltaTime)
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
		UDustCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);
		
		CameraMode->Update(DeltaTime);
		
		// 가중치가 1인 카메라 모드가 있으면 그 이후에 나오는 카메라 모드는
		// 의미가 없으므로 스택에서 제거합니다.
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
			UDustCameraMode* CameraMode = CameraModeStack[StackIndex];
			check(CameraMode);
			CameraMode->Deactivate();
		}
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

FDustCameraModeView UDustCameraModeStack::BlendStack() const
{
	const int32 StackSize = CameraModeStack.Num();
	FDustCameraModeView Result;
	
	if (StackSize > 0)
	{
		const UDustCameraMode* CameraMode = CameraModeStack[StackSize - 1];
		check(CameraMode);
		
		Result = CameraMode->GetCameraModeView();
		
		// 스택에 있는 모든 카메라 모드를 블렌딩한 결과는 스택의 마지막 카메라 모드에 저장합니다.
		for (int32 StackIndex = StackSize - 2; StackIndex >= 0; --StackIndex)
		{
			CameraMode = CameraModeStack[StackIndex];
			check(CameraMode);
			Result.Blend(CameraMode->GetCameraModeView(), CameraMode->GetBlendWeight());
		}
	}
	
	return Result;
}

bool UDustCameraModeStack::IsActive() const
{
	return bIsActive;
}

TKeyValuePair<FGameplayTag, float> UDustCameraModeStack::GetEvaluatedBlendWeight() const
{
	if (CameraModeStack.Num() == 0)
	{
		return { FGameplayTag::EmptyTag, 0.0f };
	}
	
	const UDustCameraMode* Last = CameraModeStack.Last();
	check(Last);
	
	return { Last->GetCameraTypeTag(), Last->GetBlendWeight() };
}

void UDustCameraModeStack::DrawDebug(UCanvas* Canvas) const
{
	check(Canvas);
	
	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;
	
	DisplayDebugManager.SetDrawColor(FColor::Green);
	DisplayDebugManager.DrawString(FString(TEXT("   --- Camera Modes (Begin) ---")));
	
	for (const UDustCameraMode* CameraMode : CameraModeStack)
	{
		check(CameraMode);
		CameraMode->DrawDebug(Canvas);
	}
	
	DisplayDebugManager.SetDrawColor(FColor::Green);
	DisplayDebugManager.DrawString(FString(TEXT("   --- Camera Modes (End) ---")));
}
