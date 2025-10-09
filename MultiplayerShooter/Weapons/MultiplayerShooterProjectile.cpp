#include "Weapons/MultiplayerShooterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMultiplayerShooterProjectile::AMultiplayerShooterProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}
