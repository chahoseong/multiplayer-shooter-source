#include "Weapons/MultiplayerShooterProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
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

void AMultiplayerShooterProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BoxComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	}
}

void AMultiplayerShooterProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UAbilitySystemComponent* OtherAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if (IsValid(OtherAbilitySystem) && IsValid(DamageEffectClass))
	{
		const UAbilitySystemComponent* OwningAbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
		FGameplayEffectContextHandle EffectContext = OwningAbilitySystem->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		EffectContext.AddInstigator(GetInstigator(), OwningAbilitySystem->GetOwnerActor());
		EffectContext.AddHitResult(Hit);
		const FGameplayEffectSpecHandle EffectSpec = OwningAbilitySystem->MakeOutgoingSpec(
			DamageEffectClass,
			1.0f,
			EffectContext
		);
		OtherAbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);
	}
	
	Destroy();
}
