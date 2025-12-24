#pragma once

#include "AbilitySystemComponent.h"
#include "DustAbilitySystemComponent.generated.h"


UCLASS()
class DUST_API UDustAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UDustAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
