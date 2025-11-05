#pragma once

#include "GameFramework/Actor.h"
#include "MultiplayerShooterProjectile.generated.h"

class UGameplayEffect;
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterProjectile : public AActor
{
	GENERATED_BODY()

public:
	AMultiplayerShooterProjectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Fire(const FVector& TargetLocation);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
