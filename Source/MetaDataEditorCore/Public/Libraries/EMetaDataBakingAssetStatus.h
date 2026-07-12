#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMetaDataBakingAssetStatus : uint8 {
	MDBAS_NONE,
	MDBAS_Baked UMETA(DisplayName = "Baked"),
	MDBAS_SavedOnly UMETA(DisplayName = "Saved Only"),
	MDBAS_Dirty UMETA(DisplayName = "Dirty") ,
	MDBAS_Empty UMETA(DisplayName= "No Data")
};