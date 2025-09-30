#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerShooterPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class MULTIPLAYERSHOOTER_API AMultiplayerShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMultiplayerShooterPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
};
