#pragma once

#include "DustCharacter.h"
#include "AbilitySystemInterface.h"
#include "DustPlayerCharacter.generated.h"

class UAbilitySystemComponent;
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
	
	virtual void OnRep_PlayerState() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:
	void AddInputMappingContext() const;
	void BindInputActions(UInputComponent* PlayerInputComponent);
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	
	void InitializeWithAbilitySystem();

protected:
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMapping;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UDustInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
