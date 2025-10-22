#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerShooterPlayerController.generated.h"

class AMultiplayerShooterHUD;
class UMultiplayerShooterAbilitySystemComponent;
class UMultiplayerShooterInputConfig;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMultiplayerShooterPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PlayerTick(float DeltaTime) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	virtual void OnRep_PlayerState() override;
	
	void GetHitResultFromPlayerViewPoint(FHitResult& OutHit, float MaxRange) const;
	const FHitResult& GetAimHitResult() const;

	UFUNCTION(BlueprintPure)
	AMultiplayerShooterHUD* GetMultiplayerShooterHUD() const;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Team")
	FGenericTeamId TeamId;

	UPROPERTY(EditDefaultsOnly)
	float AimMaxRange = 999999.0f;

	UPROPERTY(Transient, BlueprintReadOnly);
	FHitResult AimHitResult;
};
