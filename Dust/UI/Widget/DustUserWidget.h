#pragma once

#include "Blueprint/UserWidget.h"
#include "DustUserWidget.generated.h"

class UDustWidgetController;

UCLASS(Abstract, Blueprintable, BlueprintType)
class DUST_API UDustUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDustUserWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
