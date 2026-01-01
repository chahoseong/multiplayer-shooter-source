#include "UI/Widget/DustUserWidget.h"

UDustUserWidget::UDustUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDustUserWidget::SetController(UObject* NewController)
{
	Controller = NewController;
	OnControllerChanged(NewController);
}

UObject* UDustUserWidget::GetController() const
{
	return Controller.Get();
}
