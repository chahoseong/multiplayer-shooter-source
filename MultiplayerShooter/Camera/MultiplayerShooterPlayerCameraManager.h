#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MultiplayerShooterPlayerCameraManager.generated.h"

#define MULTIPLAYERSHOOTER_DEFAULT_FOV (80.0f)
#define MULTIPLAYERSHOOTER_DEFAULT_PITCH_MIN (-89.0f)
#define MULTIPLAYERSHOOTER_DEFAULT_PITCH_MAX (89.0f)

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AMultiplayerShooterPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
