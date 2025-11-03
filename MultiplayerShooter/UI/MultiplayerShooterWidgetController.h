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

	APlayerState* GetPlayerState() const;

	template <typename T> requires std::is_base_of_v<APlayerState, T>
	T* GetPlayerState() const
	{
		return Cast<T>(GetPlayerState());
	}
	
protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<APlayerController> PlayerController;
};
