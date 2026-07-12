#pragma once
#include "CoreMinimal.h"
#include "FMetaDataBakingSettingsAssetIndex.h"
#include "FBakingSettingsAssetIndexer.generated.h"

USTRUCT(BlueprintType)
struct FBakingSettingsAssetIndexer
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSet<FMetaDataBakingSettingsAssetIndex> IndexedAssets;
	
};
