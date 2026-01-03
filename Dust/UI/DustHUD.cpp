#include "UI/DustHUD.h"
#include "UI/Widget/DustUserWidget.h"

ADustHUD::ADustHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ADustHUD::InitializeOverlay()
{
	ActiveOverlayWidget = CreateWidget<UDustUserWidget>(GetOwningPlayerController(), OverlayWidgetType);
	ActiveOverlayWidget->AddToViewport();
}
