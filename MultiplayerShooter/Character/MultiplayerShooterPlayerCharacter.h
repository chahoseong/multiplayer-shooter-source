#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "MultiplayerShooterCharacter.h"
#include "MultiplayerShooterPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMultiplayerShooterCameraStateComponent;
class UMultiplayerShooterCameraMode;
class UMultiplayerShooterEquipmentDefinition;
class UMultiplayerShooterEquipmentManagerComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerCharacter : public AMultiplayerShooterCharacter
{
	GENERATED_BODY()

public:
	AMultiplayerShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void SetAbilityCameraMode(TSubclassOf<UMultiplayerShooterCameraMode> CameraMode,
		const FGameplayAbilitySpecHandle& OwningAbilitySpec);
	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningAbilitySpec);

	virtual void OnRep_PlayerState() override;

private:
	void InitializeAbilitySystem();
	TSubclassOf<UMultiplayerShooterCameraMode> DetermineCameraMode() const;
	void CheckDistanceToCamera();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayAbility> AimingAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
	TObjectPtr<UMultiplayerShooterEquipmentManagerComponent> EquipmentManagerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment")
	TArray<TSubclassOf<UMultiplayerShooterEquipmentDefinition>> StartupEquipments;

	UPROPERTY()
	TObjectPtr<UMultiplayerShooterCameraStateComponent> CameraStateComponent;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TSubclassOf<UMultiplayerShooterCameraMode> DefaultCameraMode;

	UPROPERTY(EditAnywhere, Category="Camera")
	float HideDistanceThreshold = 200.0f;

private:
	TSubclassOf<UMultiplayerShooterCameraMode> CurrentAbilityCameraMode;
	FGameplayAbilitySpecHandle AbilityCameraModeSource;
};
