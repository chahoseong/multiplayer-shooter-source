#include "Weapons/MultiplayerShooterProjectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Physics/MultiplayerShooterCollisionChannels.h"

AMultiplayerShooterProjectile::AMultiplayerShooterProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	// Movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	// Collision
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->SetCollisionObjectType(MultiplayerShooter_ObjectChannel_Projectile);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(MultiplayerShooter_ObjectChannel_Character, ECR_Block);
	
	// Networking
	bReplicates = true;
	SetReplicatingMovement(true);
}
