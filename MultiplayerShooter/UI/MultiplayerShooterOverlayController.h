#pragma once

#include "GenericTeamAgentInterface.h"
#include "MultiplayerShooterWidgetController.h"
#include "MultiplayerShooterOverlayController.generated.h"

struct FOnAttributeChangeData;
class UMultiplayerShooterUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCrosshairSpread, float, Base, float, Multiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetChanged, const AActor*, NewTarget, ETeamAttitude::Type, Attitude);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);

UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERSHOOTER_API UMultiplayerShooterOverlayController : public UMultiplayerShooterWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static UMultiplayerShooterOverlayController* GetOverlayController(APlayerController* InPlayerController);
	
public:
	virtual void Initialize(APlayerController* NewPlayerController) override;

	void BroadcastInitialAttributes();
	
	UFUNCTION(BlueprintCallable)
	void SetCrosshair(TSubclassOf<UMultiplayerShooterUserWidget> CrosshairWidgetClass);

	UFUNCTION(BlueprintCallable)
	void UpdateCrosshairSpread(float BaseSpreadAngle, float SpreadAngleMultiplier);
	
	void SetTarget(const AActor* NewTarget, ETeamAttitude::Type Attitude) const;
	
	UPROPERTY(BlueprintAssignable)
	FOnCrosshairSpread OnCrosshairSpread;

	UPROPERTY(BlueprintAssignable)
	FOnTargetChanged OnTargetChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnMaxHealthChanged;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UMultiplayerShooterUserWidget> ActiveCrosshairWidget;
};
