// MetaDataEditorDelegates.h (Inside Indexer Module)
#pragma once
#include "CoreMinimal.h"
#include "FExtractionResult.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMetaDataAssetBaked, const FSoftObjectPath& /*Asset*/, bool /*bSuccess*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMetaDataExtracted,  const FSoftObjectPath& /*Asset*/, FMetaDataExtractionResult /*bSuccess*/);

struct METADATAEDITORCORE_API FCoreMetaDataEditorDelegates
{
	// Fired by the Baker module when an asset's traits are successfully pushed to a registry.
	static FOnMetaDataAssetBaked OnAssetBaked;
	
	// Fired by the Baker module when an asset's traits are attempted to be extracted
	static FOnMetaDataExtracted OnMetaDataExtracted;
};