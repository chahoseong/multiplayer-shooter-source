#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MultiplayerShooterGameplayAbility.generated.h"

class UMultiplayerShooterCameraMode;

UENUM(BlueprintType)
enum class EMultiplayerShooterAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnGranted,
	Manually,
};

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMultiplayerShooterGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void TryActivateAbilityOnGiven(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
	
	EMultiplayerShooterAbilityActivationPolicy GetActivationPolicy() const;

	UFUNCTION(BlueprintPure, Category="MultiplayerShooter|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category="MultiplayerShooter|Ability", meta=(DeterminesOutputType=ControllerType))
	AController* GetTypedControllerFromActorInfo(TSubclassOf<AController> ControllerType) const;
	
	UFUNCTION(BlueprintCallable, Category="MultiplayerShooter|Ability")
	void SetCameraMode(TSubclassOf<UMultiplayerShooterCameraMode> CameraMode);

	UFUNCTION(BlueprintCallable, Category="MultiplayerShooter|Ability")
	void ClearCameraMode();
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MultiplayerShooter|Ability Activation")
	EMultiplayerShooterAbilityActivationPolicy ActivationPolicy;

	TSubclassOf<UMultiplayerShooterCameraMode> ActiveCameraMode;
};
