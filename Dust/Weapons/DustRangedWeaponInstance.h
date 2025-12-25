#pragma once

#include "CoreMinimal.h"
#include "DustWeaponInstance.h"
#include "DustRangedWeaponInstance.generated.h"

UCLASS()
class DUST_API UDustRangedWeaponInstance : public UDustWeaponInstance
{
	GENERATED_BODY()
	
public:
	UDustRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
