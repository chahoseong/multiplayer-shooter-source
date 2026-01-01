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
	
	UFUNCTION(BlueprintCallable, Category="Dust|UI")
	void SetController(UObject* NewController);
	
	UFUNCTION(BlueprintPure, Category="Dust|UI")
	UObject* GetController() const;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnControllerChanged(UObject* NewController);
	
protected:
	TWeakObjectPtr<UObject> Controller;
};
