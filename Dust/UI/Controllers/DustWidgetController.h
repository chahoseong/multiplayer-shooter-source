#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "DustWidgetController.generated.h"

class ADustPlayerState;

UCLASS(Abstract, Blueprintable)
class DUST_API UDustWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UDustWidgetController(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitializeController();
	virtual void BroadcastInitialValues() const;
	
	virtual UWorld* GetWorld() const override;
	
	APlayerController* GetPlayerController() const;
	ADustPlayerState* GetDustPlayerState() const;
};
