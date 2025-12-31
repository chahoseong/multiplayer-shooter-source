#pragma once

#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "DustCameraMode.generated.h"

class UDustCameraStateComponent;

UENUM(BlueprintType)
enum class EDustCameraModeBlendFunction : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,
};

struct FDustCameraModeView
{
	// SpringArm
	FVector SpringArmLocation;
	float TargetArmLength;
	FVector SocketOffset;
	FVector TargetOffset;
	
	// Camera
	float FieldOfView;
	
	// PlayerController
	FRotator ControlRotation;
	
	FDustCameraModeView();
	
	void Blend(const FDustCameraModeView& OtherView, float OtherWeight);
};

UCLASS(Abstract, Blueprintable)
class DUST_API UDustCameraMode : public UObject
{
	GENERATED_BODY()
	
public:
	UDustCameraMode();
	
	virtual void Activate();
	virtual void Deactivate();
	
	void Update(float DeltaTime);
	
	virtual UWorld* GetWorld() const override;
	
	UDustCameraStateComponent* GetCameraState() const;
	AActor* GetTargetActor() const;
	
	const FGameplayTag& GetCameraTypeTag() const;
	float GetBlendTime() const;
	
	float GetBlendWeight() const;
	void SetBlendWeight(float Weight);
	
	const FDustCameraModeView& GetCameraModeView() const;
	
	virtual void DrawDebug(UCanvas* Canvas) const;
	
protected:
	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);
	
	void UpdateForTarget(float DeltaTime);
	void UpdateCrouchOffset(float DeltaTime);
	void SetTargetCrouchOffset(const FVector& NewTargetOffset);
	
	virtual FRotator GetPivotRotation() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="General")
	FGameplayTag CameraTypeTag;
	
	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="5.0", UIMax="170.0", ClampMin="5.0", ClampMax="170.0"))
	float FieldOfView;
	
	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="-89.9", UIMax="89.9", ClampMin="-89.9", ClampMax="89.9"))
	float ViewPitchMin;
	
	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin="-89.9", UIMax="89.9", ClampMin="-89.9", ClampMax="89.9"))
	float ViewPitchMax;
	
	UPROPERTY(EditDefaultsOnly, Category="View")
	FVector SpringArmLocation;
	
	UPROPERTY(EditDefaultsOnly, Category="View")
	float TargetArmLength = 350.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="View")
	FVector SocketOffset;
	
	UPROPERTY(EditDefaultsOnly, Category="View")
	FVector TargetOffset;
	
	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendTime = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Blending")
	EDustCameraModeBlendFunction BlendFunction = EDustCameraModeBlendFunction::EaseOut;
	
	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendExponent = 4.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Blending")
	float CrouchOffsetBlendMultiplier = 5.0f;
	
	FDustCameraModeView View;
	
private:
	// Linear blend alpha used to determine the blend weight.
	float BlendAlpha = 1.0f;
	
	// Blend weight calculated using the blend alpha and function.
	float BlendWeight = 1.0f;
	
	FVector InitialCrouchOffset;
	FVector TargetCrouchOffset;
	float CrouchOffsetBlendAlpha = 1.0f;
	FVector CurrentCrouchOffset;
};
