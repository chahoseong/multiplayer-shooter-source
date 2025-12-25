#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DustPlayerController.generated.h"

class UDustEquipmentDefinition;

UCLASS()
class DUST_API ADustPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADustPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void OnPossess(APawn* InPawn) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TArray<TSubclassOf<UDustEquipmentDefinition>> StartupEquipments;
};
