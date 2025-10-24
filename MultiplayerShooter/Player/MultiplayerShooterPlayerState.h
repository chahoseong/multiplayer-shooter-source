#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GenericTeamAgentInterface.h"
#include "MultiplayerShooterPlayerState.generated.h"

class UMultiplayerShooterAbilitySystemComponent;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerState : public APlayerState, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMultiplayerShooterPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <typename T> requires std::is_base_of_v<UAbilitySystemComponent, UMultiplayerShooterAbilitySystemComponent>
	UMultiplayerShooterAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}
	
	UAttributeSet* GetAttributeSet() const;

	virtual void SetGenericTeamId(const FGenericTeamId& TeamId) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	UFUNCTION()
	void OnRep_MyTeamId(FGenericTeamId OldTeamId);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_MyTeamId)
	FGenericTeamId MyTeamId;
};
