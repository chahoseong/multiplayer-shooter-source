#pragma once

#include "GameFramework/Actor.h"
#include "MultiplayerShooterProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterProjectile : public AActor
{
	GENERATED_BODY()

public:
	AMultiplayerShooterProjectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
};
