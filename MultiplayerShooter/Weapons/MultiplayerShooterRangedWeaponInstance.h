#pragma once

#include "MultiplayerShooterWeaponInstance.h"
#include "MultiplayerShooterRangedWeaponInstance.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterRangedWeaponInstance : public UMultiplayerShooterWeaponInstance, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;

	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override;
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintPure)
	float GetBaseSpreadAngle() const;
	
	UFUNCTION(BlueprintPure)
	float GetSpreadAngleMultiplier() const;

private:
	void UpdateMovingMultiplier(const APawn* Pawn, float DeltaTime);
	void UpdateJumpingOrFallingMultiplier(const APawn* Pawn, float DeltaTime);
	void UpdateAimingMultiplier(const APawn* Pawn, float DeltaTime);
	void UpdateShootingMultiplier(float DeltaTime);
	
protected:
	UPROPERTY(EditAnywhere, Category="Spread")
	float BaseSpreadAngle;
	
	UPROPERTY(EditAnywhere, Category="Spread")
	float SpreadAngleMultiplier_Moving = 1.0f;

	UPROPERTY(EditAnywhere, Category="Spread")
	float TransitionRate_Moving = 5.0f;

	UPROPERTY(EditAnywhere, Category="Spread")
	float SpreadAngleMultiplier_JumpingOrFalling = 1.0f;

	UPROPERTY(EditAnywhere, Category="Spread")
	float TransitionRate_JumpingOrFalling = 5.0f;

	UPROPERTY(EditAnywhere, Category="Spread")
	float SpreadAngleMultiplier_Aiming = 1.0f;

	UPROPERTY(EditAnywhere, Category="Spread")
	float TransitionRate_Aiming = 5.0f;

	UPROPERTY(EditAnywhere, Category="Spread")
	float SpreadAngleMultiplier_Shooting = 1.0f;
	
private:
	float CurrentSpreadAngle = 0.0f;
	float CurrentSpreadAngleMultiplier = 0.0f;

	float MovingMultiplier = 0.0f;
	float JumpingOrFallingMultiplier = 0.0f;
	float AimingMultiplier = 0.0f;
	float ShootingMultiplier = 0.0f;
	
	bool bTickable = false;
};
