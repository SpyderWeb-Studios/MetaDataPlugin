// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataRuntimeSubsystem.h"

#include "Interfaces/MetaDataRegistryDataInterface.h"
#include "Libraries/Runtime/FAssetMetaDataIdSet.h"

DEFINE_LOG_CATEGORY(LogMetaDataRuntime)

void UMetaDataRuntimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FCoreUObjectDelegates::GetPostGarbageCollect().AddUObject(this, &UMetaDataRuntimeSubsystem::CacheInvalidation);
}

bool UMetaDataRuntimeSubsystem::GetAssetMetaDataSet(UObject* Asset, FAssetMetaDataIdSet& OutSet)
{
	if(AssetIdCache.Contains(Asset))
	{
		// Cache Hit - O(1)
		OutSet = AssetIdCache[Asset];
		return true;
	}

	if(!IsValid(Asset))
	{
		OutSet.AssetIds.Empty();
		return false;
	}

	// Cast Asset to Asset User Data interface - Validity Check
	IInterface_AssetUserData* AssetUserDataOwner = Cast<IInterface_AssetUserData>(Asset);
	if(!AssetUserDataOwner)
	{
		UE_LOG(LogMetaDataRuntime, Error,
			TEXT("[%s] does not implement Asset User Data interface, cannot retrieve meta data"),
			*GetNameSafe(Asset));
		return false;
	}
	
	// Retrieve the meta data user data object from the asset
	const TArray<UAssetUserData*>* AssetDataArray = AssetUserDataOwner->GetAssetUserDataArray();
	if(!AssetDataArray)
	{
		UE_LOG(LogMetaDataRuntime, Error,
			TEXT("[%s] failed to return Asset User Data Array, cannot retrieve meta data"),
			*GetNameSafe(Asset));
		return false;
	}

	FAssetMetaDataIdSet AssetMetaDataIds;
	for(const UAssetUserData* DataEntry: *AssetDataArray)
	{
		if(!DataEntry)
		{
			continue;
		}
		if(DataEntry->Implements<UMetaDataRegistryDataInterface>())
		{
			UE_LOG(LogMetaDataRuntime, Display,
				TEXT("[%s] found Registry Meta Data [%s]"),
				*GetNameSafe(Asset),
				*GetNameSafe(DataEntry));

			TSet<FDataRegistryId> LocalIds;;

			IMetaDataRegistryDataInterface::Execute_GetAllRegistryIds(DataEntry, LocalIds);

			AssetMetaDataIds.AssetIds.Append(LocalIds);
		}
	}
	

	// Cache the asset data, set the output and return true
	AssetIdCache.Add(Asset, FAssetMetaDataIdSet(AssetMetaDataIds.AssetIds));
	OutSet = AssetMetaDataIds;

	return true;
}

bool UMetaDataRuntimeSubsystem::GetAssetMetaDataSetForType(UObject* Asset, const FDataRegistryType& DataRegistryType, TSet<FDataRegistryId>& OutSet)
{
	FAssetMetaDataIdSet AllRegistrySet;
	if(!GetAssetMetaDataSet(Asset, AllRegistrySet))
	{
		UE_LOG(LogMetaDataRuntime, Warning, TEXT("Failed to retrieve Meta Data Set"));
		return false;
	}

	// Instant O(1) Hash Lookup
	if (const TSet<FDataRegistryId>* FoundSet = AllRegistrySet.SegmentedIds.Find(DataRegistryType))
	{
		OutSet = *FoundSet;
		return OutSet.Num() > 0;
	}

	return false;
}

void UMetaDataRuntimeSubsystem::CacheInvalidation()
{
	for(const TTuple<TWeakObjectPtr<UObject>, FAssetMetaDataIdSet> KeyValuePair : AssetIdCache)
	{
		if(KeyValuePair.Key.IsStale())
		{
			AssetIdCache.Remove(KeyValuePair.Key);
		}
		
	}
}
