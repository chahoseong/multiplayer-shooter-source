#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "DustHUD.generated.h"

class UDustOverlayController;
class UDustUserWidget;
class UDustWidgetController;

UCLASS()
class DUST_API ADustHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ADustHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void InitializeOverlay();
	
	UDustOverlayController* GetOverlayController() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UDustUserWidget> OverlayWidgetType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UDustOverlayController> OverlayControllerType;
	
private:
	UPROPERTY()
	TObjectPtr<UDustUserWidget> ActiveOverlayWidget;
	
	UPROPERTY()
	mutable TObjectPtr<UDustOverlayController> ActiveOverlayController;
};
