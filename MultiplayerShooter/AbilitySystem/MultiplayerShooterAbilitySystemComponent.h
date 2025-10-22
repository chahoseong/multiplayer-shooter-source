#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterAbilitySystemComponent.generated.h"

enum class EMultiplayerShooterAbilityActivationPolicy : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMultiplayerShooterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMultiplayerShooterAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	void TryActivateAbilityOnGiven();

	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

private:
	bool IsAbilityActivationPolicy(const UGameplayAbility* AbilityToCheck, EMultiplayerShooterAbilityActivationPolicy ActivationPolicy) const;

protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
