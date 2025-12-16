#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Expected.h"
#include "DustInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FDustInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Input.Action"))
	FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class DUST_API UDustInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UDustInputConfig(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category="Dust|Pawn")
	const UInputAction* FindNativeInputActionWithTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	UFUNCTION(BlueprintCallable, Category="Dust|Pawn")
	const UInputAction* FindAbilityInputActionWithTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputAction"))
	TArray<FDustInputAction> NativeInputActions;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty="InputAction"))
	TArray<FDustInputAction> AbilityInputActions;
};
