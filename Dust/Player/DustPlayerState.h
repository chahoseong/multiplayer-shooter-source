#pragma once

#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "DustPlayerState.generated.h"

class UDustHealthSet;
class UDustAbilitySystemComponent;

UCLASS()
class DUST_API ADustPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADustPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintPure, Category="Dust|Player State")
	UDustAbilitySystemComponent* GetDustAbilitySystemComponent() const;

	UDustHealthSet* GetHealthSet() const;
	
protected:
	UPROPERTY()
	TObjectPtr<UDustAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UDustHealthSet> HealthSet;
};
