#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DustPlayerCameraManager.generated.h"

#define DUST_CAMERA_DEFAULT_FOV (80.0f)
#define DUST_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define DUST_CAMERA_DEFAULT_PITCH_MAX (89.0f)

UCLASS()
class DUST_API ADustPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ADustPlayerCameraManager(const FObjectInitializer& ObjectInitializer);
};
