#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DustRangedWeaponActor.generated.h"

class UDustAnimInstance;

USTRUCT()
struct FWeaponEffector
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FName SocketName;
	
	UPROPERTY(EditDefaultsOnly)
	FVector TargetLocation;
};

UCLASS()
class DUST_API ADustRangedWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	ADustRangedWeaponActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void Tick(float DeltaSeconds) override;

private:
	UDustAnimInstance* GetOwnerAnimInstance() const;
	bool IsAiming() const;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="IK")
	FWeaponEffector GripEffector;
};
