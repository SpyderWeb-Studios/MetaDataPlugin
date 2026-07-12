// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataIndexerSubsystem.h"

#include "JsonObjectConverter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "DataAssets/BakingSettings/MetaDataBakingSettingsDataAsset.h"
#include "Engine/AssetManager.h"
#include "Libraries/FCoreMetaDataEditorDelegates.h"

DEFINE_LOG_CATEGORY(LogMetaDataIndexer);

UMetaDataIndexerSubsystem::UMetaDataIndexerSubsystem()
{
}



void UMetaDataIndexerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    RefreshIndex();
	FCoreMetaDataEditorDelegates::OnAssetBaked.AddUObject(this, &UMetaDataIndexerSubsystem::HandleAssetBaked);
	FCoreMetaDataEditorDelegates::OnMetaDataExtracted.AddUObject(this, &UMetaDataIndexerSubsystem::HandleMetaDataExtracted);
}

void UMetaDataIndexerSubsystem::RefreshIndex()
{
    {
        FARFilter Filter;
        Filter.ClassPaths.Add(UMetaDataBakingSettingsDataAsset::StaticClass()->GetClassPathName());
        Filter.bRecursiveClasses = true;

        TArray<FAssetData> AssetDataList;
        FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry")
            .Get().GetAssets(Filter, AssetDataList);

        UE_LOG(LogTemp, Log, TEXT("Found %d assets"), AssetDataList.Num());
    
        for(const FAssetData& AssetData : AssetDataList)
        {
            UE_LOG(LogMetaDataIndexer, Log, TEXT("Adding [%s] to Cache"), *AssetData.GetObjectPathString());
            CachedBakingSettingsDataAssets.Add(
                TSoftObjectPtr<UMetaDataBakingSettingsDataAsset>(AssetData.GetSoftObjectPath()));
        
        }
    }
    
    UE_LOG(LogMetaDataIndexer, Log, TEXT("Cached %d assets. Indexing....."), CachedBakingSettingsDataAssets.Num());

    for(const TSoftObjectPtr<UMetaDataBakingSettingsDataAsset>& Settings : CachedBakingSettingsDataAssets)
    {
    	RefreshDataAssetCache(Settings.LoadSynchronous());
    }
    
}

void UMetaDataIndexerSubsystem::GetDirectoryIndex(const FDirectoryPath& DirectoryPath, TSet<TSoftObjectPtr<UMetaDataBakingSettingsDataAsset>>& OutIndex)
{

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Setup the filter
	FARFilter Filter;
	// DirectoryPath.Path usually provides the string path
	Filter.PackagePaths.Add(FName(*DirectoryPath.Path));
	Filter.bRecursivePaths = true;
    
	// Filter by the specific class to ensure we only get the data assets we want
	Filter.ClassPaths.Add(UMetaDataBakingSettingsDataAsset::StaticClass()->GetClassPathName());

	// Query
	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssets(Filter, AssetDataList);

	// Convert FAssetData to TSoftObjectPtr and add to the set
	for (const FAssetData& AssetData : AssetDataList)
	{
		// Create a soft pointer from the asset data
		TSoftObjectPtr<UMetaDataBakingSettingsDataAsset> SoftPtr(AssetData.ToSoftObjectPath());
        
		if (SoftPtr.IsValid() || !SoftPtr.IsNull())
		{
			OutIndex.Add(SoftPtr);
		}
	}
}

EMetaDataBakingAssetStatus UMetaDataIndexerSubsystem::GetAssetStatus(UObject* Asset) const
{
	// Guard against null assets
	if (!Asset)
	{
		return EMetaDataBakingAssetStatus::MDBAS_Empty; 
	}

	FMetaDataBakingSettingsAssetIndex Index;
	Index.Asset = Asset;

	// Safely check if Find() returned a valid pointer before dereferencing
	if (const FMetaDataBakingSettingsAssetIndex* FoundIndex = CachedIndex.Find(Index))
	{
		return FoundIndex->AssetStatus;
	}

	// Not found in the index at all
	return EMetaDataBakingAssetStatus::MDBAS_Empty;
}

void UMetaDataIndexerSubsystem::RefreshDataAssetCache(UMetaDataBakingSettingsDataAsset* DataAsset)
{
	FString TargetFolder = FPackageName::GetLongPackagePath(FSoftObjectPath(DataAsset).GetAssetPath().GetPackageName().ToString());
	
    UE_LOG(LogMetaDataIndexer, Log, TEXT("Searching for assets in [%s]"), *TargetFolder);
    FARFilter Filter;
    Filter.PackagePaths.Add(FName(TargetFolder));
    Filter.bRecursivePaths = true;
        
    TArray<FAssetData> AssetDataList;
    FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry")
        .Get().GetAssets(Filter, AssetDataList);

	// Blank Potential Cache
	TSet<TSoftObjectPtr<UObject>> PotentialCache;

    for(const FAssetData& AssetData : AssetDataList)
	{
	    // Get the class of the asset without loading the asset itself
	    UClass* AssetClass = AssetData.GetClass();
    
	    // Check if it implements the interface at the class level
	    if (AssetClass && AssetClass->ImplementsInterface(UInterface_AssetUserData::StaticClass()))
	    {
	        // This is a "Candidate" - it CAN hold user data. 
	        PotentialCache.Add(TSoftObjectPtr<UObject>(AssetData.ToSoftObjectPath()));

	    	FMetaDataBakingSettingsAssetIndex Index;
	    	Index.Asset = AssetData.ToSoftObjectPath();
	    	Index.AssetStatus = EMetaDataBakingAssetStatus::MDBAS_Dirty;

	    	if(!CachedIndex.Contains(Index))
	    	{
	    		CachedIndex.Add(Index);
	    	}
	    }
	}

	DataAsset->AssignCache(PotentialCache);
	DataAsset->MarkPackageDirty();

}

void UMetaDataIndexerSubsystem::HandleAssetBaked(UObject* Asset, bool bSuccess)
{
	UE_LOG(LogMetaDataIndexer, Display, TEXT("[%s] Received Asset [%s] Baked [%s]"),
		*GetNameSafe(this),
		*GetNameSafe(Asset),
		( bSuccess ? TEXT("Successfully"): TEXT("Failed") ));


	// Guard against null assets
	if (!Asset)
	{
		return; 
	}

	FMetaDataBakingSettingsAssetIndex Index;
	Index.Asset = Asset;

	// Safely check if Find() returned a valid pointer before dereferencing
	if (FMetaDataBakingSettingsAssetIndex* FoundIndex = CachedIndex.Find(Index))
	{
		FoundIndex->AssetStatus = bSuccess ? EMetaDataBakingAssetStatus::MDBAS_Baked : EMetaDataBakingAssetStatus::MDBAS_SavedOnly;
	}
	
}

void UMetaDataIndexerSubsystem::HandleMetaDataExtracted(UObject* Asset,
	FMetaDataExtractionResult MetaDataExtractionResult)
{
	UE_LOG(LogMetaDataIndexer, Display, TEXT("[%s] Received Asset [%s] Extracted Traits"),
		*GetNameSafe(this),
		*GetNameSafe(Asset));


	// Guard against null assets
	if (!Asset)
	{
		return; 
	}

	FMetaDataBakingSettingsAssetIndex Index;
	Index.Asset = Asset;

	// Safely check if Find() returned a valid pointer before dereferencing
	if (FMetaDataBakingSettingsAssetIndex* FoundIndex = CachedIndex.Find(Index))
	{
		FoundIndex->AssetStatus = !MetaDataExtractionResult.bHasTraits ? EMetaDataBakingAssetStatus::MDBAS_Empty : EMetaDataBakingAssetStatus::MDBAS_Dirty;
	}
}
