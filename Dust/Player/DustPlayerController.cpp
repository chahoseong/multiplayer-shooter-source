#include "Player/DustPlayerController.h"
#include "Equipment/DustEquipmentManagerComponent.h"

ADustPlayerController::ADustPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ADustPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UDustEquipmentManagerComponent* EquipmentManager = 
		InPawn->FindComponentByClass<UDustEquipmentManagerComponent>();
	if (EquipmentManager)
	{
		for (TSubclassOf Equipment : StartupEquipments)
		{
			EquipmentManager->Equip(Equipment);
		}	
	}
}
