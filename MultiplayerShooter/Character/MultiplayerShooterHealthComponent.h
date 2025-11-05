#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/ActorComponent.h"
#include "MultiplayerShooterHealthComponent.generated.h"

struct FOnAttributeChangeData;
class UMultiplayerShooterHealthSet;
class UAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMultiplayerShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMultiplayerShooterHealthComponent();

	void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);
	void ReleaseFromAbilitySystem(UAbilitySystemComponent* AbilitySystem);

protected:
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	
protected:
	UPROPERTY()
	TObjectPtr<UMultiplayerShooterHealthSet> HealthSet;
};
