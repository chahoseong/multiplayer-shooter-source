#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DustCameraStateComponent.generated.h"


class UDustCameraMode;
class UCameraComponent;
class USpringArmComponent;
class UDustCameraModeStack;

DECLARE_DELEGATE_RetVal(TSubclassOf<UDustCameraMode>, FDustCameraModeDelegate);

UCLASS()
class DUST_API UDustCameraStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDustCameraStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;
	
	AActor* GetTargetActor() const;
	
	FDustCameraModeDelegate DetermineCameraModeDelegate;
	
private:
	void UpdateCameraModes() const;
	
protected:
	UPROPERTY()
	TObjectPtr<UDustCameraModeStack> CameraModeStack;
	
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;
};
