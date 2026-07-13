// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataIndexerSubsystem.h"
#include "Libraries/FCoreMetaDataEditorDelegates.h"
#include "DataAssets/BakingSettings/MetaDataBakingSettingsDataAsset.h"
#include "Engine/AssetManager.h"

#include "Serialization/BufferArchive.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"

DEFINE_LOG_CATEGORY(LogMetaDataIndexer);

UMetaDataIndexerSubsystem::UMetaDataIndexerSubsystem()
{
	CacheFile = FPaths::ProjectSavedDir() / "MetaDataBaker" / TEXT("Cache.bin");
}



void UMetaDataIndexerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	DeserialiseIndex();
	
	FCoreMetaDataEditorDelegates::OnAssetBaked.AddUObject(this, &UMetaDataIndexerSubsystem::HandleAssetBaked);
	FCoreMetaDataEditorDelegates::OnMetaDataExtracted.AddUObject(this, &UMetaDataIndexerSubsystem::HandleMetaDataExtracted);
}

void UMetaDataIndexerSubsystem::Deinitialize()
{
	SerialiseIndex();
	
	Super::Deinitialize();
	
}

void UMetaDataIndexerSubsystem::RefreshIndex()
{

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	if (AssetRegistry.IsLoadingAssets())
	{
		UE_LOG(LogMetaDataIndexer, Warning, TEXT("Asset Registry is still loading. Forcing synchronous scan of /Game/ to find DataAssets..."));
		TArray<FString> BasePaths = { TEXT("/Game") }; // Scans the main content folder
		AssetRegistry.ScanPathsSynchronous(BasePaths, true);
	}
	
	TSet<FString> TargetFolders;
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
        	
        	TargetFolders.Add(AssetData.PackagePath.ToString());
            UE_LOG(LogMetaDataIndexer, Log, TEXT("Adding [%s] to TargetFolders"),
            	*AssetData.PackagePath.ToString());
        }
    }

	
	// This ensures that all unloaded assets on disk are discovered and indexed before we run our filter.
	AssetRegistry.ScanPathsSynchronous(TargetFolders.Array(), true);
    
    UE_LOG(LogMetaDataIndexer, Log, TEXT("Cached %d assets. Indexing....."), CachedBakingSettingsDataAssets.Num());
	CachedIndex.Empty();

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

EMetaDataBakingAssetStatus UMetaDataIndexerSubsystem::GetSoftAssetStatus(const FSoftObjectPath& SoftAsset) const
{
	
	// Guard against null assets
	if (SoftAsset.IsNull())
	{
		return EMetaDataBakingAssetStatus::MDBAS_NONE; 
	}

	FMetaDataBakingSettingsAssetIndex Index;
	Index.Asset = SoftAsset;

	// Safely check if Find() returned a valid pointer before dereferencing
	if (const FMetaDataBakingSettingsAssetIndex* FoundIndex = CachedIndex.Find(Index))
	{
		return FoundIndex->AssetStatus;
	}

	// Not found in the index at all
	return EMetaDataBakingAssetStatus::MDBAS_NONE;
}

TSoftObjectPtr<UMetaDataBakingSettingsDataAsset> UMetaDataIndexerSubsystem::GetBakingSettingForAsset(
	const FSoftObjectPath& SoftAsset)
{

	FMetaDataBakingSettingsAssetIndex IndexToFind(SoftAsset);
	if(FMetaDataBakingSettingsAssetIndex* Found = CachedIndex.Find(SoftAsset))
	{
		return Found->OwningBakingSetting;
	}
	return nullptr;
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
	    UClass* AssetClass = AssetData.GetClass(EResolveClass::Yes);
    
    	UE_LOG(LogMetaDataIndexer, Display, TEXT("[%s] Caching index [%s][%s]"),
			*GetNameSafe(DataAsset),
			*AssetData.ToSoftObjectPath().ToString(),
			( AssetClass ? TEXT("Resolved"): TEXT("UnResolved") ));
    	
	    // Check if it implements the interface at the class level
	    if (AssetClass && AssetClass->ImplementsInterface(UInterface_AssetUserData::StaticClass()))
	    {
	    	
	    	UE_LOG(LogMetaDataIndexer, Display, TEXT("[%s] : [%s] : Is Valid and Implements Asset User Data"),
				*GetNameSafe(DataAsset),
				*AssetData.ToSoftObjectPath().ToString());
	    	
	        // This is a "Candidate" - it CAN hold user data. 
	        PotentialCache.Add(TSoftObjectPtr<UObject>(AssetData.ToSoftObjectPath()));

	    	FMetaDataBakingSettingsAssetIndex Index;
	    	Index.Asset = AssetData.ToSoftObjectPath();
	    	Index.AssetStatus = EMetaDataBakingAssetStatus::MDBAS_Indexed;
	    	Index.OwningBakingSetting = DataAsset;
	    	CachedIndex.Add(Index);
	    		    	
	    }
	}

	DataAsset->AssignCache(PotentialCache);
	DataAsset->MarkPackageDirty();
}

void UMetaDataIndexerSubsystem::SerialiseIndex()
{
	FBufferArchive ToBinary;
	FObjectAndNameAsStringProxyArchive Ar(ToBinary, false);

	// Manually serialize the number of elements
	int32 NumElements = CachedIndex.Num();
	Ar << NumElements;

	// Iterate and serialize each element
	for (const FMetaDataBakingSettingsAssetIndex& Element : CachedIndex)
	{
		// We create a temporary copy to serialize
		FMetaDataBakingSettingsAssetIndex Temp = Element;
		Ar << Temp;
	}

	FFileHelper::SaveArrayToFile(ToBinary, *CacheFile);
}

void UMetaDataIndexerSubsystem::DeserialiseIndex()
{

	TArray<uint8> BinaryData;
	if (FFileHelper::LoadFileToArray(BinaryData, *CacheFile))
	{
		FMemoryReader FromBinary(BinaryData, true);
		FObjectAndNameAsStringProxyArchive Ar(FromBinary, true);

		int32 NumElements = 0;
		Ar << NumElements;

		CachedIndex.Empty();
		for (int32 i = 0; i < NumElements; ++i)
		{
			FMetaDataBakingSettingsAssetIndex Temp;
			Ar << Temp;
			CachedIndex.Add(Temp);
		}
	}
	else
	{
		// RefreshIndex();
	}
}

void UMetaDataIndexerSubsystem::HandleAssetBaked( const FSoftObjectPath& Asset, bool bSuccess)
{
	UE_LOG(LogMetaDataIndexer, Display, TEXT("[%s] Received Asset [%s] Baked [%s]"),
		*GetNameSafe(this),
		*Asset.ToString(),
		( bSuccess ? TEXT("Successfully"): TEXT("Failed") ));


	// Guard against null assets
	if (Asset.IsNull())
	{
		return; 
	}
	

	FMetaDataBakingSettingsAssetIndex Index;
	Index.Asset = Asset;

	// Safely check if Find() returned a valid pointer before dereferencing
	if (FMetaDataBakingSettingsAssetIndex* FoundIndex = CachedIndex.Find(Index))
	{
		UpdateAssetIndexStatus(Asset, bSuccess ? EMetaDataBakingAssetStatus::MDBAS_Baked : EMetaDataBakingAssetStatus::MDBAS_Indexed);
	}
	
}

void UMetaDataIndexerSubsystem::HandleMetaDataExtracted( const FSoftObjectPath& Asset,
	FMetaDataExtractionResult MetaDataExtractionResult)
{
	UE_LOG(LogMetaDataIndexer, Display, TEXT("[%s] Received Asset [%s] Extracted Traits"),
		*GetNameSafe(this),
		*Asset.ToString());

	// Guard against null assets
	if (Asset.IsNull())
	{
		return; 
	}

	FMetaDataBakingSettingsAssetIndex Index;
	Index.Asset = Asset;

	// Safely check if Find() returned a valid pointer before dereferencing
	if (FMetaDataBakingSettingsAssetIndex* FoundIndex = CachedIndex.Find(Index))
	{
		if(!MetaDataExtractionResult.bHasTraits)
		{
			UpdateAssetIndexStatus(Asset, EMetaDataBakingAssetStatus::MDBAS_Empty);
		}
		else
		{
			UpdateAssetIndexStatus(Asset, EMetaDataBakingAssetStatus::MDBAS_Indexed);
		}
	}

}

void UMetaDataIndexerSubsystem::UpdateAssetIndexStatus(const FSoftObjectPath& Asset, EMetaDataBakingAssetStatus Status)
{
	// Guard against null assets
	if (Asset.IsNull())
	{
		return; 
	}

	FMetaDataBakingSettingsAssetIndex Index;
	Index.Asset = Asset;

	// Safely check if Find() returned a valid pointer before dereferencing
	if (FMetaDataBakingSettingsAssetIndex* FoundIndex = CachedIndex.Find(Index))
	{
		FoundIndex->AssetStatus = Status;
	}
}
