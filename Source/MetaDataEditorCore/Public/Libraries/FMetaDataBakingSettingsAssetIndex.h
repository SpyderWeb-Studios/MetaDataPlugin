#pragma  once

#include "CoreMinimal.h"
#include "EMetaDataBakingAssetStatus.h"
#include "FMetaDataBakingSettingsAssetIndex.generated.h"


USTRUCT(BlueprintType)
struct FMetaDataBakingSettingsAssetIndex
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TSoftObjectPtr<UObject> Asset;

	UPROPERTY(VisibleAnywhere)
	EMetaDataBakingAssetStatus AssetStatus;
	
	// Required for TSet: Defines what makes two entries "equal"
	bool operator==(const FMetaDataBakingSettingsAssetIndex& Other) const
	{
		// Two indexes are the same if they point to the same asset
		return Asset == Other.Asset; 
	}

	// Required for TSet: Generates a unique hash for the container
	friend uint32 GetTypeHash(const FMetaDataBakingSettingsAssetIndex& Item)
	{
		// We hash the SoftObjectPtr directly
		return GetTypeHash(Item.Asset); 
	}

	// Required for Binary Serialisation
	friend FArchive& operator<<(FArchive& Ar, FMetaDataBakingSettingsAssetIndex& Index)
	{
		Ar << Index.Asset;
		Ar << Index.AssetStatus;
		return Ar;
	}
};
