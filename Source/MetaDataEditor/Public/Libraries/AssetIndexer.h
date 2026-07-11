#pragma  once

#include "CoreMinimal.h"
#include "AssetIndexer.generated.h"

USTRUCT(BlueprintType)
struct FAssetIndexer
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSet<FSoftObjectPath> IndexedAssets;
};
