#pragma once

#include "DustCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "DustPlayerCharacter.generated.h"

class UDustHealthSet;
class UDustCameraMode;
class UDustCameraStateComponent;
class UDustEquipmentDefinition;
class UDustEquipmentManagerComponent;
class UDustAbilitySystemComponent;
class UCameraComponent;
class USpringArmComponent;
class UDustInputComponent;
class UDustInputConfig;
struct FInputActionValue;
class UInputMappingContext;

UCLASS()
class DUST_API ADustPlayerCharacter : public ADustCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADustPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void SetCameraMode(TSubclassOf<UDustCameraMode> CameraMode, UObject* SourceObject);
	void ClearCameraMode(UObject* SourceObject);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void OnRep_PlayerState() override;

private:
	void InitializeCharacter();
	void InitializeWithAbilitySystem();
	void InitializeCameraState();
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	
	TSubclassOf<UDustCameraMode> DetermineCameraMode() const;

protected:
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<UDustCameraStateComponent> CameraStateComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TSubclassOf<UDustCameraMode> DefaultCameraMode;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMapping;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UDustInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UDustAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UDustHealthSet> HealthSet;
	
	UPROPERTY(VisibleAnywhere, Category="Equipment")
	TObjectPtr<UDustEquipmentManagerComponent> EquipmentManagerComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<TSubclassOf<UDustEquipmentDefinition>> StartupEquipments;
	
private:
	TSubclassOf<UDustCameraMode>  CurrentCameraMode;
	TWeakObjectPtr<UObject> CameraModeSource;
};
