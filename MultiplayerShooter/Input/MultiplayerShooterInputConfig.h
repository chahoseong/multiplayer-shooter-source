#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MultiplayerShooterInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FMultiplayerShooterInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Input.Action"))
	FGameplayTag InputTag;
};

UCLASS()
class MULTIPLAYERSHOOTER_API UMultiplayerShooterInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UMultiplayerShooterInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="MultiplayerShooter|Pawn")
	const UInputAction* FindNativeInputActionWithTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category="MultiplayerShooter|Pawn")
	const UInputAction* FindAbilityInputActionWithTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputAction"))
	TArray<FMultiplayerShooterInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputAction"))
	TArray<FMultiplayerShooterInputAction> AbilityInputActions;
};
