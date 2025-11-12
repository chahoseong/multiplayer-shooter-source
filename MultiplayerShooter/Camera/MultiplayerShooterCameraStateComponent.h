#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MultiplayerShooterCameraStateComponent.generated.h"

struct FGameplayAbilitySpecHandle;
class UMultiplayerShooterCameraMode;
class UMultiplayerShooterCameraModeStack;
class UCameraComponent;
class USpringArmComponent;

DECLARE_DELEGATE_RetVal(TSubclassOf<UMultiplayerShooterCameraMode>, FMultiplayerShooterCameraModeDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMultiplayerShooterCameraStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="MultiplayerShooter|Camera")
	static UMultiplayerShooterCameraStateComponent* FindCameraStateComponent(const AActor* Actor);
	
public:
	UMultiplayerShooterCameraStateComponent();

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FMultiplayerShooterCameraModeDelegate DetermineCameraModeDelegate;
	
protected:
	void UpdateCameraModes() const;

private:
	bool IsOwnerLocallyControlled() const;
	
protected:
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<UMultiplayerShooterCameraModeStack> CameraModeStack;
};
