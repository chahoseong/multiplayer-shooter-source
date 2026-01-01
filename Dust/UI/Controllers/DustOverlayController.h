#pragma once

#include "CoreMinimal.h"
#include "DustWidgetController.h"
#include "DustOverlayController.generated.h"

struct FGameplayEffectSpec;
struct FDustVerbMessage;

UCLASS()
class DUST_API UDustOverlayController : public UDustWidgetController
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeEvent, float, NewValue);
	
public:
	UDustOverlayController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void InitializeController() override;
	virtual void BroadcastInitialValues() const override;
	
	UPROPERTY(BlueprintAssignable)
	FAttributeEvent OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FAttributeEvent OnMaxHealthChanged;
};
