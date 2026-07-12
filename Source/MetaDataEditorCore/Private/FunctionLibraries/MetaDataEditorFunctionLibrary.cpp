// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/MetaDataEditorFunctionLibrary.h"
#include <AssetRegistry/AssetRegistryModule.h>

#include "Interfaces/MetaDataExportInterface.h"

DEFINE_LOG_CATEGORY(LogMetaDataEditorCore)


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

    TArray<FAssetData> AssetList;
    AssetRegistry.GetAssets(Filter, AssetList);

    // 3. Iterate and check for the interface
    for (const FAssetData& Asset : AssetList)
    {
        UClass* AssetClass = Asset.GetClass(EResolveClass::Yes);

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
