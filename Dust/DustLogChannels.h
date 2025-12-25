#pragma once

#include "Logging/LogMacros.h"

DUST_API DECLARE_LOG_CATEGORY_EXTERN(LogDust, Log, All)
DUST_API DECLARE_LOG_CATEGORY_EXTERN(LogDustAbilitySystem, Log, All)

DUST_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
