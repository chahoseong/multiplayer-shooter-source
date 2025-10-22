#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MultiplayerShooterCameraModeStack.generated.h"

struct FGameplayTag;
struct FMultiplayerShooterCameraModeView;
class UMultiplayerShooterCameraMode;

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	void ActivateStack();
	void DeactivateStack();

	void PushCameraMode(TSubclassOf<UMultiplayerShooterCameraMode> CameraModeClass);
	bool EvaluateStack(float DeltaTime, FMultiplayerShooterCameraModeView& OutCameraModeView);
	
	void GetCameraModeBlendWeight(float& OutBlendWeight, FGameplayTag& OutCameraModeTag);

	bool IsActive() const;

protected:
	void UpdateStack(float DeltaTime);
	void BlendStack(FMultiplayerShooterCameraModeView& OutCameraModeView) const;
	
	UMultiplayerShooterCameraMode* GetCameraModeInstance(TSubclassOf<UMultiplayerShooterCameraMode> CameraModeClass) const;
	UMultiplayerShooterCameraMode* Peek() const;
	bool IsTop(const UMultiplayerShooterCameraMode* CameraMode) const;
	bool TryGetCameraMode(UMultiplayerShooterCameraMode* CameraMode, int32& OutIndex, float& OutStackContribution) const;
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<UMultiplayerShooterCameraMode>> CameraModeStack;

	UPROPERTY()
	mutable TArray<TObjectPtr<UMultiplayerShooterCameraMode>> CameraModeInstances;

	bool bIsActive = true;
};
