#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MultiplayerShooterWidgetController.generated.h"

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterWidgetController : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(APlayerController* NewPlayerController);

protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<APlayerController> PlayerController;
};
