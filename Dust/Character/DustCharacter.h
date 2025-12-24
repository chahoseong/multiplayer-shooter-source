#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DustCharacter.generated.h"

UCLASS()
class DUST_API ADustCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADustCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
