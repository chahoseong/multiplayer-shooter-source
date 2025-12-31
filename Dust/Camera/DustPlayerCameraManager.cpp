#include "Camera/DustPlayerCameraManager.h"

ADustPlayerCameraManager::ADustPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = DUST_CAMERA_DEFAULT_FOV;
	ViewPitchMin = DUST_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = DUST_CAMERA_DEFAULT_PITCH_MAX;
}
