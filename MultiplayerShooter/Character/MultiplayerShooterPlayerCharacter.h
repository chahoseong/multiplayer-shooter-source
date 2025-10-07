#pragma once

#include "CoreMinimal.h"
#include "MultiplayerShooterCharacter.h"
#include "MultiplayerShooterPlayerCharacter.generated.h"

class UMultiplayerShooterEquipmentDefinition;
class UMultiplayerShooterEquipmentManagerComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerCharacter : public AMultiplayerShooterCharacter
{
	GENERATED_BODY()

public:
	AMultiplayerShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

private:
	void InitializeAbilitySystem();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayAbility> AimingAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
	TObjectPtr<UMultiplayerShooterEquipmentManagerComponent> EquipmentManagerComponent;

	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<TSubclassOf<UMultiplayerShooterEquipmentDefinition>> StartupEquipments;
};
