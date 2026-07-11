// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/MetaDataEditorFunctionLibrary.h"
#include <AssetRegistry/AssetRegistryModule.h>

#include "Interfaces/MetaDataExportInterface.h"
#include "Subsystems/MetaDataEditorSubsystem.h"

DEFINE_LOG_CATEGORY(LogMetaDataBakerEditor)


bool UMetaDataEditorFunctionLibrary::ScanForBakingSettings(const FDirectoryPath& RootFolder, TMap<UMetaDataBakingSettingsDataAsset*, FName>& OutBakingSettings)
{
    OutBakingSettings.Empty();

    // 1. Sanitize the Input Path (Remove trailing slash)
    FString SanitizedPath = RootFolder.Path;
    if (SanitizedPath.EndsWith(TEXT("/")))
    {
        SanitizedPath.RemoveFromEnd(TEXT("/"));
    }

    UE_LOG(LogMetaDataBakerEditor, Display, TEXT("MetadataBaker: Scanning recursively for settings in path: %s"), *SanitizedPath);

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
        UE_LOG(LogMetaDataBakerEditor, Warning, TEXT("MetadataBaker: No UMetaDataBakingSettingsDataAsset found in %s or its subfolders. Ensure they are created and saved."), *SanitizedPath);
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
            UE_LOG(LogMetaDataBakerEditor, Error, TEXT("MetadataBaker: ABORTING BAKE. Found multiple Settings Assets in the same directory: %s. Only ONE Settings Asset is allowed per folder to prevent routing conflicts."), *FolderPath.ToString());
            return false;
        }

        // 6. Load and map the asset
        if (UMetaDataBakingSettingsDataAsset* LoadedAsset = Cast<UMetaDataBakingSettingsDataAsset>(AssetData.GetAsset()))
        {
            OutBakingSettings.Add(LoadedAsset, FolderPath);
            ProcessedFolders.Add(FolderPath);

            UE_LOG(LogMetaDataBakerEditor, Display, TEXT("MetadataBaker: Discovered Settings Asset for folder [%s] -> %s"), *FolderPath.ToString(), *AssetData.AssetName.ToString());
        }
    }

    return OutBakingSettings.Num() > 0;
}

void UMetaDataEditorFunctionLibrary::ProcessBakeForDirectory(const FDirectoryPath& Directory)
{
    TMap<UMetaDataBakingSettingsDataAsset*, FName> BakingSettings;

    if (UMetaDataEditorFunctionLibrary::ScanForBakingSettings(Directory, BakingSettings))
    {
        UE_LOG(LogMetaDataBakerEditor, Display, TEXT("MetadataBaker: Found %d baking settings for folder: %s"), BakingSettings.Num(), *Directory.Path);
        
        for (const TPair< UMetaDataBakingSettingsDataAsset*, FName> SettingPaths : BakingSettings)
        { 
            TArray<FAssetData> RelevantAssets;
            FindAssetUserDataOwners(FDirectoryPath(SettingPaths.Value.ToString()), RelevantAssets);

            UE_LOG(LogMetaDataBakerEditor, Display, TEXT("MetadataBaker: Processing [%d] Assets in [%s]"), RelevantAssets.Num(), *SettingPaths.Value.ToString());
            
            for (const FAssetData& AssetData : RelevantAssets)
            {
                if (UObject* LoadedAsset = AssetData.GetAsset())
                {
                    BakeMetadataForAsset(LoadedAsset);
                }
            }

        }
        
        GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>()->FlushAllModifiedStorageProviders();
    }
    else
    {
        UE_LOG(LogMetaDataBakerEditor, Warning, TEXT("MetadataBaker: No valid settings found in %s. Skipping."), *Directory.Path);
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

bool UMetaDataEditorFunctionLibrary::BakeMetadataForAsset(UObject* Asset)
{
    if (!Asset)
    {
        return false;
    }

    // Check if the asset (or its internal components) implements your interface [cite: 104]
    IInterface_AssetUserData* MetadataInterface = Cast<IInterface_AssetUserData>(Asset);
    
    // If the saved object is completely unrelated to your trait pipeline, exit cleanly without overhead
    if (!MetadataInterface)
    {
        return false;
    }
    
    FDirectoryPath TargetFolder;
    TargetFolder.Path = FPaths::GetPath(Asset->GetPathName());

    TMap<UMetaDataBakingSettingsDataAsset*, FName> FoundSettings;
    ScanForBakingSettings(TargetFolder, FoundSettings);

    if(FoundSettings.IsEmpty())
    {
        UE_LOG(LogMetaDataBakerEditor, Warning, TEXT("MetadataBaker: Failed to locate a Baking Settings asset managing path: %s"), *TargetFolder.Path);
        return false;
    }
    
   UMetaDataBakingSettingsDataAsset* ResolvedSettings = ResolveSettingsForAsset(Asset->GetPathName(), FoundSettings);

    if(!IsValid(ResolvedSettings))
    {
        UE_LOG(LogMetaDataBakerEditor, Warning, TEXT("MetadataBaker: Failed to locate a Baking Settings asset managing path: %s"), *TargetFolder.Path);
        return false;
    }
    
    const TArray<UAssetUserData*>* MetaData = MetadataInterface->GetAssetUserDataArray();
    if(!MetaData || MetaData->Num() == 0)
    {
        UE_LOG(LogMetaDataBakerEditor, Warning, TEXT("MetaData Baker : No Asset User Data found on [%s]"), *Asset->GetFName().ToString());
        return false;
    }

    // Ensure the designer selected a naming convention in the settings
    if (!ResolvedSettings->GetNamingConvention())
    {
        UE_LOG(LogTemp, Error, TEXT("MetaData Baker: No Naming Convention configured in Settings Asset [%s]"), *ResolvedSettings->GetName());
        return false;
    }

    FName RegistryKey = ResolvedSettings->GetNamingConvention()->GenerateRegistryKey(Asset->GetPathName()); 

    bool bBakeSucceeded = true;
    UMetaDataEditorSubsystem* MetaDataEditorSubsystem = GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>();
    
    UE_LOG(LogTemp, Error, TEXT("MetaData Baker: [%s] Extracting Meta Data"), *ResolvedSettings->GetName());
    for (UAssetUserData* UserData : *MetaData)
    {
        if(!UserData->Implements<UMetaDataExportInterface>())
        {
            continue;
        }
        
        TArray<TInstancedStruct<FMetaDataTrait_Base>> ExtractedTraits;
        IMetaDataExportInterface::Execute_ExportTraits(UserData, ExtractedTraits, true);
        
        
        UE_LOG(LogTemp, Error, TEXT("MetaData Baker: [%s] Extracted [%d] Meta Data Entries"), *ResolvedSettings->GetName(), ExtractedTraits.Num());
        
        for (TInstancedStruct<FMetaDataTrait_Base> Trait : ExtractedTraits)
        {
            if (!Trait.IsValid()) continue;

            // Identify the trait type for O(1) routing
            const UScriptStruct* TraitType = Trait.GetScriptStruct(); 

            // Look up the Struct type in your Settings Map
            FMetadataProviderArray TargetGroup = ResolvedSettings->GetTraitProviders(TraitType);
            
            // Iterate through the array of providers and write
            for (UMetaDataStorageProvider_Base* Provider : TargetGroup.ProviderArray)
            {
                if (Provider)
                {
                    // Hand the payload to the resolved provider 
                    if (!IMetaDataStorageProviderInterface::Execute_ProcessMetadata(Provider, RegistryKey, Trait, Asset)) 
                    {
                        bBakeSucceeded = false;
                    }

                    MetaDataEditorSubsystem->MarkProviderModified(Provider);
                    
                }
                
            }
        }
        
    }
    
    return bBakeSucceeded;
}


UMetaDataBakingSettingsDataAsset* UMetaDataEditorFunctionLibrary::ResolveSettingsForAsset(const FString& TargetAssetPath,
                                                             const TMap<UMetaDataBakingSettingsDataAsset*, FName>&
                                                             SettingsMap)
{

    UE_LOG(LogMetaDataBakerEditor, Log, TEXT("Resolving Settings for [%s] : [%d] Found"), *TargetAssetPath, SettingsMap.Num());

    // Start with the folder containing the asset
    FString CurrentSearchPath = FPaths::GetPath(TargetAssetPath); 

    // Walk up the directory tree until we hit the root
    while (!CurrentSearchPath.IsEmpty() && CurrentSearchPath != TEXT("/"))
    {
        FName SearchName = FName(*CurrentSearchPath);
        
        UE_LOG(LogMetaDataBakerEditor, Log, TEXT("Searching [%s] for Settings"), *CurrentSearchPath);
        
        // Iterate to find the matching folder path in the map's values
        for (const TPair<UMetaDataBakingSettingsDataAsset*, FName>& Pair : SettingsMap)
        {
            
            UE_LOG(LogMetaDataBakerEditor, Log, TEXT("Comparing [%s] : [%s]"), *Pair.Value.ToString(), *SearchName.ToString());
            if (Pair.Value == SearchName)
            {
                return Pair.Key; // Found the closest parent configuration!
            }
        }

        // Move up one directory level (e.g., "/Game/Gothic/Walls" -> "/Game/Gothic")
        CurrentSearchPath = FPaths::GetPath(CurrentSearchPath);
    }

    return nullptr; // No settings found anywhere in the parent hierarchy
}