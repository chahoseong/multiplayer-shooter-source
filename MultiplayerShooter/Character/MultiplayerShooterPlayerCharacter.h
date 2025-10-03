#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooterCharacter.h"
#include "MultiplayerShooterPlayerCharacter.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerCharacter : public AMultiplayerShooterCharacter
{
	GENERATED_BODY()

public:
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

private:
	void InitializeAbilitySystem();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayAbility> AimingAbility;
};
