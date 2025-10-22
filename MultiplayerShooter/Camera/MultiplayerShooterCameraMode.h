#pragma once

#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "MultiplayerShooterCameraMode.generated.h"

class UMultiplayerShooterCameraComponent;

UENUM(BlueprintType)
enum class EMultiplayerShooterCameraModeBlendFunction : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT UMETA(Hidden)
};

struct FMultiplayerShooterCameraModeView
{
	FRotator ControlRotation;
	FVector SocketOffset;
	float TargetArmLength;
	float FieldOfView;
	
	FMultiplayerShooterCameraModeView();
	void Blend(const FMultiplayerShooterCameraModeView& Other, float Alpha);
};

UCLASS(Abstract, Blueprintable)
class MULTIPLAYERSHOOTER_API UMultiplayerShooterCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UMultiplayerShooterCameraMode();

	virtual void OnActivation();
	virtual void OnDeactivation();
	
	void UpdateCameraMode(float DeltaTime);
	
	const FGameplayTag& GetCameraModeTag() const;
	const FMultiplayerShooterCameraModeView& GetCameraModeView() const;
	float GetBlendTime() const;
	
	float GetBlendWeight() const;
	void SetBlendWeight(float Weight);

	AActor* GetTargetActor() const;

protected:
	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);

	FRotator GetViewRotation() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CameraModeTag;
	
	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="5.0", UIMax="170.0", ClampMin="5.0", ClampMax="170.0"))
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="-89.0", UIMax="89.0", ClampMin="-89.0", ClampMax="89.0"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="-89.0", UIMax="89.0", ClampMin="-89.0", ClampMax="89.0"))
	float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category="View")
	FVector SocketOffset;

	UPROPERTY(EditDefaultsOnly, Category="View")
	float TargetArmLength;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	EMultiplayerShooterCameraModeBlendFunction BlendFunction = EMultiplayerShooterCameraModeBlendFunction::EaseOut;

	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendExponent = 4.0f;

private:
	FMultiplayerShooterCameraModeView CameraModeView;
	float BlendAlpha = 1.0f;
	float BlendWeight = 1.0f;
};
