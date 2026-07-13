#pragma once

#include "CoreMinimal.h"
#include "DataRegistryId.h"
#include "FAssetMetaDataIdSet.generated.h"



USTRUCT(BlueprintType)
struct FAssetMetaDataIdSet
{
	GENERATED_BODY()

	// Reflected for standard Blueprint access
	UPROPERTY(VisibleAnywhere)
	TSet<FDataRegistryId> AssetIds;

	// Pure C++ Segmented Index (No UPROPERTY to bypass UHT limitation)
	TMap<FDataRegistryType, TSet<FDataRegistryId>> SegmentedIds;

	// Constructor handles the segmentation automatically
	FAssetMetaDataIdSet(const TSet<FDataRegistryId>& Ids)
	{
		AssetIds = Ids;
	
		// Pre-compute the O(1) hash map during cache population
		for (const FDataRegistryId& Id : Ids)
		{
			SegmentedIds.FindOrAdd(Id.RegistryType).Add(Id);
		}
	}

	FAssetMetaDataIdSet() {}
	
};