#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "DustHUD.generated.h"

class UDustUserWidget;

UCLASS()
class DUST_API ADustHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ADustHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void InitializeOverlay();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UDustUserWidget> OverlayWidgetType;
	
private:
	UPROPERTY()
	TObjectPtr<UDustUserWidget> ActiveOverlayWidget;
};
