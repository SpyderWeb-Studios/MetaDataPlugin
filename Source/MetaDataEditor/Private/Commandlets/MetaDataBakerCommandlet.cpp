// Fill out your copyright notice in the Description page of Project Settings.


#include "Commandlets/MetaDataBakerCommandlet.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/MetaDataEditorSubsystem.h"
#include "FileHelpers.h" // Essential for SavePackage
#include "FunctionLibraries/MetaDataBakingFunctionLibrary.h"
#include "FunctionLibraries/MetaDataEditorFunctionLibrary.h"
#include "UObject/SavePackage.h"


int32 UMetaDataBakerCommandlet::Main(const FString& Params)
{
    UE_LOG(LogTemp, Display, TEXT(""));
    UE_LOG(LogTemp, Display, TEXT("======================================="));
    UE_LOG(LogTemp, Display, TEXT("   TRAIT BAKING PIPELINE INITIALIZED   "));
    UE_LOG(LogTemp, Display, TEXT("======================================="));
    UE_LOG(LogTemp, Display, TEXT("   Timestamp : %s"), *FDateTime::Now().ToString());
    UE_LOG(LogTemp, Display, TEXT("   Project   : %s"), *FPaths::GetProjectFilePath());
    UE_LOG(LogTemp, Display, TEXT("   Platform  : %s"), *FString(FPlatformProperties::PlatformName()));
    UE_LOG(LogTemp, Display, TEXT("--------------------------------------"));
    UE_LOG(LogTemp, Display, TEXT(""));


    UMetaDataEditorSubsystem* MetaDataSubsystem = GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>();
    if(!IsValid(MetaDataSubsystem)) return -1;

    TArray<FAssetData> RelevantContent;
    
    FString AssetRoot;
    // FParse::Value will populate AssetRoot with the string following "-AssetRoot="
    // Note: It looks for the key including the equals sign in the text argument.
    if (FParse::Value(*Params, TEXT("AssetRoot="), AssetRoot))
    {
        ScanContentForMeshes(FDirectoryPath(AssetRoot), RelevantContent);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MetadataBaker: Missing required argument: -AssetRoot (eg. /Game/ )"));
        return -1; // Fail gracefully if no path is provided
    }
    
    UE_LOG(LogTemp, Display, TEXT("Found %d meshes to process."), RelevantContent.Num());

    int32 Count = 0;
    const int32 GC_Threshold = 50; // Trigger Garbage Collection every 50 assets

    for (const FAssetData& Data : RelevantContent)
    {
        // 1. Get the actual object (loads it into memory)
        UObject* LoadedAsset = Data.GetAsset();
        
        if (LoadedAsset)
        {
            UMetaDataBakingFunctionLibrary::BakeMetadataForAsset(LoadedAsset);
        }

        // 3. Periodic Garbage Collection
        if (++Count % GC_Threshold == 0)
        {
            UE_LOG(LogTemp, Display, TEXT("Batch processing: %d / %d. Running GC..."), Count, RelevantContent.Num());
            
            // This clears out objects that are no longer referenced after BakeMetadataForAsset
            CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
        }
    }

    // Final GC
    CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);

    UE_LOG(LogTemp, Display, TEXT("=============================================="));
    UE_LOG(LogTemp, Display, TEXT("   TRAIT BATCHING COMPLETE. SAVING PACKAGES   "));
    UE_LOG(LogTemp, Display, TEXT("=============================================="));
    
    
    return 0;
    
}

void UMetaDataBakerCommandlet::ScanContentForMeshes(const FDirectoryPath& RootFolder, TArray<FAssetData>& OutAssetData)
{
    UE_LOG(LogTemp, Log, TEXT("Scanning Content in [%s]"), *RootFolder.Path);
    UMetaDataEditorFunctionLibrary::FindAssetUserDataOwners(RootFolder, OutAssetData);
}


void UMetaDataBakerCommandlet::CommitChanges(TSet<UObject*> ModifiedObjects)
{
    for (UObject* Table : ModifiedObjects)
    {
        if (!Table) continue;

        UPackage* Package = Table->GetOutermost();
        FString PackageFileName = FPackageName::LongPackageNameToFilename(
            Package->GetName(), 
            FPackageName::GetAssetPackageExtension()
        );

        // Define save arguments for UE5
        FSavePackageArgs SaveArgs;
        SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
        SaveArgs.Error = GError;
        SaveArgs.bForceByteSwapping = true;
        SaveArgs.bWarnOfLongFilename = true;

        if (UPackage::Save(Package, Table, *PackageFileName, SaveArgs).IsSuccessful())
        {
            UE_LOG(LogTemp, Display, TEXT("Successfully saved [%s]"), *GetNameSafe(Table));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save %s"), *GetNameSafe(Table));
        }
    }

}    

