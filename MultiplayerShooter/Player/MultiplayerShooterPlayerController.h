#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerShooterPlayerController.generated.h"

class UMultiplayerShooterAbilitySystemComponent;
class UMultiplayerShooterInputConfig;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMultiplayerShooterPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UMultiplayerShooterAbilitySystemComponent* GetMultiplayerShooterAbilitySystemComponent() const;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UMultiplayerShooterInputConfig> InputConfig;
};
