#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MultiplayerShooterCharacter.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMultiplayerShooterCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
