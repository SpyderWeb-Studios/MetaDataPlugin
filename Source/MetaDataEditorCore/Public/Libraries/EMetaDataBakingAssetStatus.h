#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMetaDataBakingAssetStatus : uint8 {
	MDBAS_NONE = 0,
	MDBAS_Empty = 1 UMETA(DisplayName= "No Data"),
	MDBAS_Indexed  = 2 UMETA(DisplayName = "Indexed") ,
	MDBAS_SavedOnly = 3 UMETA(DisplayName = "Saved Only"),
	MDBAS_Baked = 4 UMETA(DisplayName = "Baked"),
};