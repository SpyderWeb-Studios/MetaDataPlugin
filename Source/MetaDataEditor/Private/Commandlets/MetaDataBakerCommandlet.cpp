// Fill out your copyright notice in the Description page of Project Settings.


#include "Commandlets/MetaDataBakerCommandlet.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/MetaDataEditorSubsystem.h"
#include "FileHelpers.h" // Essential for SavePackage
#include "FunctionLibraries/MetaDataEditorFunctionLibrary.h"
#include "Subsystems/MetaDataIndexerSubsystem.h"
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
    UMetaDataIndexerSubsystem* MetaDataIndexer = GEditor->GetEditorSubsystem<UMetaDataIndexerSubsystem>();

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

