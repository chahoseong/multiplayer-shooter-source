#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MultiplayerShooterAttributeSet.h"
#include "MultiplayerShooterCombatSet.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterCombatSet : public UMultiplayerShooterAttributeSet
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UMultiplayerShooterCombatSet, AttackDamage);
	
protected:
	UFUNCTION()
	void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage);
	
protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackDamage)
	FGameplayAttributeData AttackDamage;
};
