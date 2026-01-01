#pragma once

#include "GameFramework/PlayerController.h"
#include "DustPlayerController.generated.h"

class UDustAbilitySystemComponent;

UCLASS()
class DUST_API ADustPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADustPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
protected:
	UFUNCTION(BlueprintPure, Category="Dust|Player Controller")
	UDustAbilitySystemComponent* GetDustAbilitySystemComponent() const;
};
