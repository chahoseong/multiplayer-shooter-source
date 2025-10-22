#include "Camera/MultiplayerShooterPlayerCameraManager.h"

AMultiplayerShooterPlayerCameraManager::AMultiplayerShooterPlayerCameraManager(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = MULTIPLAYERSHOOTER_DEFAULT_FOV;
	ViewPitchMin = MULTIPLAYERSHOOTER_DEFAULT_PITCH_MIN;
	ViewPitchMax = MULTIPLAYERSHOOTER_DEFAULT_PITCH_MAX;
}

void AMultiplayerShooterPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
}
