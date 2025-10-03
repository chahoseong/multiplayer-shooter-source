#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MultiplayerShooterCharacterMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FMultiplayerShooterCharacterGroundInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance = 0.0f;

	uint64 LastUpdateFrame = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIPLAYERSHOOTER_API UMultiplayerShooterCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UMultiplayerShooterCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

	virtual void SimulateMovement(float DeltaTime) override;
	virtual bool CanAttemptJump() const override;

	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;

	UFUNCTION(BlueprintCallable, Category="MultiplayerShooter|CharacterMovement")
	const FMultiplayerShooterCharacterGroundInfo& GetGroundInfo() const;

	void SetReplicatedAcceleration(const FVector& InAcceleration);

protected:
	mutable FMultiplayerShooterCharacterGroundInfo CachedGroundInfo;

	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;
};
