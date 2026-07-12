// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataEditorSubsystem.h"
#include "InstancedStruct.h"
#include "JsonObjectConverter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DataAssets/BakingSettings/MetaDataBakingSettingsDataAsset.h"
#include "FunctionLibraries/MetaDataBakingFunctionLibrary.h"
#include "Interfaces/MetaDataStorageProviderInterface.h"
#include "UObject/ObjectSaveContext.h"
#include "Libraries/FMetaDataRegistryItem.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"
#include "Interfaces/MetaDataExportInterface.h"
#include "Libraries/FCoreMetaDataEditorDelegates.h"
#include "Subsystems/MetaDataIndexerSubsystem.h"
#include "Widgets/Notifications/SProgressBar.h"

DEFINE_LOG_CATEGORY(LogMetaDataEditorSubsystem);

UMetaDataEditorSubsystem::UMetaDataEditorSubsystem()
{
}


void UMetaDataEditorSubsystem::MarkProviderModified(UObject* StorageProviderInterface)
{
    ModifiedStorageProviders.Add(StorageProviderInterface);
}

void UMetaDataEditorSubsystem::FlushAllModifiedStorageProviders()
{
    for (UObject* ProviderObj : ModifiedStorageProviders)
    {
        if (!IsValid(ProviderObj))
        {
            UE_LOG(LogMetaDataEditorSubsystem, Warning, TEXT("Modified Provider [%s] is not valid"), *GetNameSafe(ProviderObj));
            continue;
        }

        if(!ProviderObj->Implements<UMetaDataStorageProviderInterface>())
        {
            UE_LOG(LogMetaDataEditorSubsystem, Warning, TEXT("Modified Provider [%s] does not implement IMetaDataStorageProviderInterface"), *GetNameSafe(ProviderObj));
            continue;
        }
        
        IMetaDataStorageProviderInterface::Execute_Flush(ProviderObj);
    }
    
    // Clear the set after processing
    ModifiedStorageProviders.Empty();
}


void UMetaDataEditorSubsystem::RequestDirectoriesBake(const TArray<FDirectoryPath>& DirectoryPaths)
{
    // Create a shared flag to track cancellation safely within the lambda
    TSharedPtr<bool> bIsCanceled = MakeShared<bool>(false);

    FNotificationInfo BakingNotification(FText::FromString("Baking Meta Data - Target Scopes"));
    BakingNotification.bFireAndForget = false;
    BakingNotification.bUseThrobber = true;
    BakingNotification.FadeOutDuration = 2.0f; // Slightly longer fade for readability

    // The Cancel Button
    BakingNotification.ButtonDetails.Add(FNotificationButtonInfo(
        FText::FromString("Cancel"),
        FText::FromString("Abort the baking process"),
        FSimpleDelegate::CreateLambda([bIsCanceled]()
        {
            // When clicked, flip our shared flag to true
            *bIsCanceled = true; 
        }),
        SNotificationItem::CS_Pending
    ));

    // This creates the standard "Toast" notification in the Editor
    TSharedPtr<SNotificationItem> BakingNotify = FSlateNotificationManager::Get().AddNotification(BakingNotification);
    if (BakingNotify.IsValid())
    {
        BakingNotify->SetCompletionState(SNotificationItem::CS_Pending);
    }
    FScopedTransaction Transaction(FText::FromString("Bake Metadata"));
    
    for (int i = 0; i < DirectoryPaths.Num(); i++)
    {
        // Fail-Fast: Check if the user clicked Cancel
        if (*bIsCanceled)
        {
            UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: Bake canceled by user at index %d."), i);
            Transaction.Cancel();
            break;
        }

        const FDirectoryPath& Dir = DirectoryPaths[i];
        
        // Optional: Update the sub text to show current progress
        if (BakingNotify.IsValid())
        {
            BakingNotify->SetSubText(FText::FromString(FString::Printf(TEXT("Baking: %s"), *Dir.Path)));
        }

        BakeDirectory(Dir);

        // This forces Unreal to momentarily process Slate inputs (like our Cancel click) 
        // before starting the next heavy directory iteration.
        FGenericPlatformApplicationMisc::PumpMessages(true);
    }

    // Resolve the Notification State
    if (BakingNotify.IsValid())
    {
        if (*bIsCanceled)
        {
            BakingNotify->SetCompletionState(SNotificationItem::CS_Fail);
            BakingNotify->SetSubText(FText::FromString("Baking Aborted"));
        }
        else
        {
            BakingNotify->SetCompletionState(SNotificationItem::CS_Success);
            BakingNotify->SetSubText(FText::FromString("Meta Data Baking Completed"));
        }
        
        BakingNotify->ExpireAndFadeout();
    }
}

void UMetaDataEditorSubsystem::BakeDirectory(const FDirectoryPath& Directory)
{


    UE_LOG(LogMetaDataEditorSubsystem, Display, TEXT("Baking Directory [%s]"), *Directory.Path);
    TSet<TSoftObjectPtr<UMetaDataBakingSettingsDataAsset>> Settings;
    GEditor->GetEditorSubsystem<UMetaDataIndexerSubsystem>()->GetDirectoryIndex(Directory, Settings);

    for(const TSoftObjectPtr<UMetaDataBakingSettingsDataAsset> Setting : Settings)
    {
        BakeBakerySetting(Setting.LoadSynchronous());
    }
}

bool UMetaDataEditorSubsystem::BakeBakerySetting(UMetaDataBakingSettingsDataAsset* BakerySetting)
{
    for(const TSoftObjectPtr<UObject> Asset : BakerySetting->CachedAssets)
    {
        BakeCachedAsset(BakerySetting, Asset);
    }

    return true;
}

bool UMetaDataEditorSubsystem::BakeCachedAsset(const UMetaDataBakingSettingsDataAsset* BakerySetting, const TSoftObjectPtr<UObject>& Asset)
{
    if (!Asset)
    {
        return false;
    }

    // Validate the injected parameter
    if (!IsValid(BakerySetting))
    {
        UE_LOG(LogMetaDataBakingLibrary, Warning, TEXT("MetadataBaker: Invalid Baking Settings provided for Asset: %s"), *Asset->GetName());
        return false;
    }

    // Ensure the designer selected a naming convention in the settings
    if (!BakerySetting->GetNamingConvention())
    {
        UE_LOG(LogMetaDataBakingLibrary, Error, TEXT("MetaData Baker: No Naming Convention configured in Settings Asset [%s]"), *BakerySetting->GetName());
        return false;
    }

    // Extract Traits using the new helper
    TArray<TInstancedStruct<FMetaDataTrait_Base>> ExtractedTraits;
    if (!ExtractAssetTraits(Asset, ExtractedTraits))
    {
        // Extraction failed or had no relevant user data
        return false;
    }

    UE_LOG(LogMetaDataBakingLibrary, Log,
        TEXT("MetaData Baker: [%s] Extracted [%d] Meta Data Entries"),
        *BakerySetting->GetName(),
        ExtractedTraits.Num());

    // Process Data
    FName RegistryKey = BakerySetting->GetNamingConvention()->GenerateRegistryKey(Asset->GetPathName()); 
    UMetaDataEditorSubsystem* MetaDataEditorSubsystem = GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>();
    bool bBakeSucceeded = true;
    
    for (const TInstancedStruct<FMetaDataTrait_Base>& Trait : ExtractedTraits)
    {
        if (!Trait.IsValid()) 
        {
            continue;
        }

        // Identify the trait type for O(1) routing
        const UScriptStruct* TraitType = Trait.GetScriptStruct(); 

        // Look up the Struct type in your Settings Map
        FMetadataProviderArray TargetGroup = BakerySetting->GetTraitProviders(TraitType);
        
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

                if (MetaDataEditorSubsystem)
                {
                    MetaDataEditorSubsystem->MarkProviderModified(Provider);
                }
            }
        }
    }

    FCoreMetaDataEditorDelegates::OnAssetBaked.Broadcast(Asset.LoadSynchronous(), bBakeSucceeded);
    return bBakeSucceeded;
}

bool UMetaDataEditorSubsystem::ExtractAssetTraits(const TSoftObjectPtr<UObject>& Asset,
    TArray<TInstancedStruct<FMetaDataTrait_Base>> OutTraits)
{
    IInterface_AssetUserData* MetadataInterface = Cast<IInterface_AssetUserData>(Asset.LoadSynchronous());
    
    // If the saved object is completely unrelated to your trait pipeline, exit cleanly
    if (!MetadataInterface)
    {
        return false;
    }


    FMetaDataExtractionResult Result;
    Result.bHasTraits = false;
    Result.bTraitsSuccessfullyExtracted = false;
    
    const TArray<UAssetUserData*>* MetaData = MetadataInterface->GetAssetUserDataArray();
    if (!MetaData || MetaData->Num() == 0)
    {
        UE_LOG(LogMetaDataBakingLibrary, Warning, TEXT("MetaData Baker : No Asset User Data found on [%s]"), *Asset->GetFName().ToString());
        // Need to broadcast the attempt of the extraction
        FCoreMetaDataEditorDelegates::OnMetaDataExtracted.Broadcast(Asset.LoadSynchronous(), Result);
        return false;
    }

    bool bFoundValidData = false;

    for (UAssetUserData* UserData : *MetaData)
    {
        if (UserData && UserData->Implements<UMetaDataExportInterface>())
        {
            bFoundValidData = true;
            IMetaDataExportInterface::Execute_ExportTraits(UserData, OutTraits, true);
        }
    }

    Result.bHasTraits = OutTraits.Num() > 0;
    Result.bTraitsSuccessfullyExtracted = bFoundValidData;

    FCoreMetaDataEditorDelegates::OnMetaDataExtracted.Broadcast(Asset.LoadSynchronous(), Result);
    return bFoundValidData;
}


TArray<UDataTable*> UMetaDataEditorSubsystem::GetAllMetadataTables()
{
    TArray<UDataTable*> FoundTables;
    
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> AssetDataList;
    
    // Search for all Data Tables
    AssetRegistryModule.Get().GetAssetsByClass(UDataTable::StaticClass()->GetClassPathName(), AssetDataList);

    for (const FAssetData& AssetData : AssetDataList)
    {
        // Only load the table if it uses our specific Registry Item Struct
        FString RowStructName;
        if (AssetData.GetTagValue(FName("RowStructure"), RowStructName))
        {
            if (RowStructName.Contains(TEXT("MetaDataRegistryItem")))
            {
                if (UDataTable* Table = Cast<UDataTable>(AssetData.GetAsset()))
                {
                    FoundTables.Add(Table);
                }
            }
        }
    }
    return FoundTables;
}
