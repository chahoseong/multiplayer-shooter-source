#pragma once

#include "MultiplayerShooterWidgetController.h"
#include "MultiplayerShooterOverlayController.generated.h"

class UMultiplayerShooterUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCrosshairSpread, float, Base, float, Multiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetChanged, AActor*, NewTarget);

UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERSHOOTER_API UMultiplayerShooterOverlayController : public UMultiplayerShooterWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static UMultiplayerShooterOverlayController* GetOverlayController(APlayerController* InPlayerController);
	
public:
	UFUNCTION(BlueprintCallable)
	void SetCrosshair(TSubclassOf<UMultiplayerShooterUserWidget> CrosshairWidgetClass);

	UFUNCTION(BlueprintCallable)
	void UpdateCrosshairSpread(float BaseSpreadAngle, float SpreadAngleMultiplier);

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* NewTarget);
	
	UPROPERTY(BlueprintAssignable)
	FOnCrosshairSpread OnCrosshairSpread;

	UPROPERTY(BlueprintAssignable)
	FOnTargetChanged OnTargetChanged;
	
protected:
	UPROPERTY(Transient)
	TObjectPtr<UMultiplayerShooterUserWidget> ActiveCrosshairWidget;
};
