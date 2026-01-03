#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DustMessageLibrary.generated.h"

UCLASS()
class DUST_API UDustMessageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Dust|Message")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);
	
	UFUNCTION(BlueprintCallable, Category="Dust|Message")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);
};
