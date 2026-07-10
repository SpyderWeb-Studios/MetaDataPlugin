// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/MetaDataEditorFunctionLibrary.h"
#include <AssetRegistry/AssetRegistryModule.h>


bool UMetaDataEditorFunctionLibrary::ScanForBakingSettings(const FDirectoryPath& RootFolder, TMap<UMetaDataBakingSettingsDataAsset*, FName>& OutBakingSettings)
{
    OutBakingSettings.Empty();

    // 1. Sanitize the Input Path (Remove trailing slash)
    FString SanitizedPath = RootFolder.Path;
    if (SanitizedPath.EndsWith(TEXT("/")))
    {
        SanitizedPath.RemoveFromEnd(TEXT("/"));
    }

    UE_LOG(LogTemp, Display, TEXT("MetadataBaker: Scanning recursively for settings in path: %s"), *SanitizedPath);

    // 2. Get the Asset Registry
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    // 3. Setup the recursive filter
    FARFilter Filter;
    Filter.PackagePaths.Add(*SanitizedPath);
    Filter.bRecursivePaths = true;
    Filter.ClassPaths.Add(UMetaDataBakingSettingsDataAsset::StaticClass()->GetClassPathName());

    // 4. Search the registry
    TArray<FAssetData> AssetList;
    AssetRegistry.GetAssets(Filter, AssetList);

    if (AssetList.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: No UMetaDataBakingSettingsDataAsset found in %s or its subfolders. Ensure they are created and saved."), *SanitizedPath);
        return false;
    }

    // 5. Track processed folders to enforce the "One Per Folder" rule
    TSet<FName> ProcessedFolders;

    for (const FAssetData& AssetData : AssetList)
    {
        FName FolderPath = AssetData.PackagePath;

        // Fail Fast: Check for Duplicates IN THE SAME FOLDER
        if (ProcessedFolders.Contains(FolderPath))
        {
            UE_LOG(LogTemp, Error, TEXT("MetadataBaker: ABORTING BAKE. Found multiple Settings Assets in the same directory: %s. Only ONE Settings Asset is allowed per folder to prevent routing conflicts."), *FolderPath.ToString());
            return false;
        }

        // 6. Load and map the asset
        if (UMetaDataBakingSettingsDataAsset* LoadedAsset = Cast<UMetaDataBakingSettingsDataAsset>(AssetData.GetAsset()))
        {
            OutBakingSettings.Add(LoadedAsset, FolderPath);
            ProcessedFolders.Add(FolderPath);

            UE_LOG(LogTemp, Display, TEXT("MetadataBaker: Discovered Settings Asset for folder [%s] -> %s"), *FolderPath.ToString(), *AssetData.AssetName.ToString());
        }
    }

    return OutBakingSettings.Num() > 0;
}

void UMetaDataEditorFunctionLibrary::ProcessBakeForDirectory(const FDirectoryPath& Directory)
{
    TMap<UMetaDataBakingSettingsDataAsset*, FName> BakingSettings;

    if (UMetaDataEditorFunctionLibrary::ScanForBakingSettings(Directory, BakingSettings))
    {
        UE_LOG(LogTemp, Display, TEXT("MetadataBaker: Found %d baking settings for folder: %s"), BakingSettings.Num(), *Directory.Path);

        // TODO: Iterate through BakingSettings map. 
        // For each Settings Asset, use the Asset Registry to find all Static Meshes 
        // inside its specific FName directory, extract traits, and route to providers.

        for (const TPair< UMetaDataBakingSettingsDataAsset*, FName> SettingPaths : BakingSettings)
        { 
            TArray<FAssetData> RelevantAssets;
            FindAssetUserDataOwners(FDirectoryPath(SettingPaths.Value.ToString()), RelevantAssets);

            UE_LOG(LogTemp, Display, TEXT("MetadataBaker: Processing [%d] Assets in [%s]"), RelevantAssets.Num(), *SettingPaths.Value.ToString());

            for (const FAssetData& Asset : RelevantAssets)
            {

            }

        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: No valid settings found in %s. Skipping."), *Directory.Path);
    }
}

void UMetaDataEditorFunctionLibrary::FindAssetUserDataOwners(const FDirectoryPath& Directory, TArray<FAssetData>& OutAssetData)
{

    OutAssetData.Empty();

    // 1. Get the Asset Registry Module
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    // 2. Setup a Filter to limit the search scope
    FARFilter Filter;
    Filter.PackagePaths.Add(*Directory.Path);
    Filter.bRecursivePaths = true;

    // Optional: If you only care about Static Meshes, filter by ClassPath
    // Filter.ClassPaths.Add(UStaticMesh::StaticClass()->GetClassPathName());

    TArray<FAssetData> AssetList;
    AssetRegistry.GetAssets(Filter, AssetList);

    // 3. Iterate and check for the interface
    for (const FAssetData& Asset : AssetList)
    {
        // Get the class of the asset without fully loading the asset if possible
        // Note: Asset.GetClass() returns the class if it's already in memory
        UClass* AssetClass = Asset.GetClass();

        // If not in memory, you may need to call GetAsset() or use the ClassPath
        if (!AssetClass)
        {
            AssetClass = Asset.GetClass(); // This might return null if the asset is not loaded
        }

        // Check for interface implementation
        if (AssetClass && AssetClass->ImplementsInterface(UInterface_AssetUserData::StaticClass()))
        {
            OutAssetData.Add(Asset);
        }
    }
}