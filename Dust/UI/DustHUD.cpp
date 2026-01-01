#include "UI/DustHUD.h"
#include "UI/Controllers/DustOverlayController.h"
#include "UI/Widget/DustUserWidget.h"

ADustHUD::ADustHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ADustHUD::InitializeOverlay()
{
	UDustOverlayController* OverlayController = GetOverlayController();
	ActiveOverlayController->InitializeController();
	
	ActiveOverlayWidget = CreateWidget<UDustUserWidget>(GetOwningPlayerController(), OverlayWidgetType);
	ActiveOverlayWidget->SetController(OverlayController);
	
	ActiveOverlayController->BroadcastInitialValues();
	
	ActiveOverlayWidget->AddToViewport();
}

UDustOverlayController* ADustHUD::GetOverlayController() const
{
	if (!ActiveOverlayController)
	{
		ActiveOverlayController = 
			NewObject<UDustOverlayController>(const_cast<ADustHUD*>(this), OverlayControllerType);
	}
	
	return ActiveOverlayController;
}
