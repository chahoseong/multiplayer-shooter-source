#include "UI/MultiplayerShooterUserWidget.h"

void UMultiplayerShooterUserWidget::SetWidgetController(UObject* NewWidgetController)
{
	WidgetController = NewWidgetController;
	OnWidgetControllerChanged(NewWidgetController);
}
