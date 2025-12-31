#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DustCameraModeStack.generated.h"

struct FGameplayTag;
struct FDustCameraModeView;
class UDustCameraMode;

UCLASS()
class DUST_API UDustCameraModeStack : public UObject
{
	GENERATED_BODY()
	
public:
	UDustCameraModeStack();
	
	void ActivateStack();
	void DeactivateStack();
	
	void PushCameraMode(TSubclassOf<UDustCameraMode> CameraModeType);
	FDustCameraModeView EvaluateStack(float DeltaTime);
	
	bool IsActive() const;
	TKeyValuePair<FGameplayTag, float> GetEvaluatedBlendWeight() const;
	
	void DrawDebug(UCanvas* Canvas) const;
	
protected:
	void UpdateStack(float DeltaTime);
	FDustCameraModeView BlendStack() const;
	
	UDustCameraMode* GetCameraModeInstance(TSubclassOf<UDustCameraMode> CameraModeType) const;
	
private:
	UPROPERTY()
	mutable TArray<TObjectPtr<UDustCameraMode>> CameraModeInstances;
	
	UPROPERTY()
	TArray<TObjectPtr<UDustCameraMode>> CameraModeStack;
	
	bool bIsActive = true;
};
