// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataEditorSubsystem.h"
#include "InstancedStruct.h"
#include "JsonObjectConverter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DataAssets/BakingSettings/MetaDataBakingSettingsDataAsset.h"
#include "Interfaces/MetaDataStorageProviderInterface.h"
#include "UObject/ObjectSaveContext.h"
#include "Libraries/FMetaDataRegistryItem.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"
#include "Interfaces/MetaDataExportInterface.h"
#include "Libraries/FCoreMetaDataEditorDelegates.h"
#include "Libraries/FMetaScopedBakeSession.h"
#include "Objects/AssetUserData/MetaDataRegistryIdData.h"
#include "Subsystems/MetaDataIndexerSubsystem.h"

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
    FScopedTransaction FlushStorage(FText::FromString("Flush Modified Storage"));
    
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
    FMetaScopedBakeSession MultiDirectorySession;
    
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

        if(!BakeDirectory(Dir))
        {
            Transaction.Cancel();
            break;
        }

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

void UMetaDataEditorSubsystem::RequestBakerySettingBake(UMetaDataBakingSettingsDataAsset* DataAssetPath) const
{
    BakeBakerySetting(DataAssetPath);
}

void UMetaDataEditorSubsystem::RequestAssetBake(const UMetaDataBakingSettingsDataAsset* BakerySetting,
                                                const FSoftObjectPath& Asset) const
{
    BakeCachedAsset(BakerySetting, TSoftObjectPtr<UObject>(Asset));
}

bool UMetaDataEditorSubsystem::BakeDirectory(const FDirectoryPath& Directory) const
{
    FMetaScopedBakeSession DirectorySession;

    UE_LOG(LogMetaDataEditorSubsystem, Display, TEXT("Baking Directory [%s]"), *Directory.Path);
    TSet<TSoftObjectPtr<UMetaDataBakingSettingsDataAsset>> Settings;
    GEditor->GetEditorSubsystem<UMetaDataIndexerSubsystem>()->GetDirectoryIndex(Directory, Settings);

    for(const TSoftObjectPtr<UMetaDataBakingSettingsDataAsset> Setting : Settings)
    {
        if(!BakeBakerySetting(Setting.LoadSynchronous()))
        {
            return false;
        }
    }

    return true;
}

bool UMetaDataEditorSubsystem::BakeBakerySetting(UMetaDataBakingSettingsDataAsset* BakerySetting) const
{
    FMetaScopedBakeSession BakerySession;

    // Clear the Provider Caches
    for(const auto& TraitRoute : BakerySetting->GetTraitRoutingMap())
    {
        for(const FMetaDataStorageProviderEntry& Provider : TraitRoute.Value.ProviderArray)
        {
            IMetaDataStorageProviderInterface::Execute_ClearCache(Provider.Provider);
        }
    }

    for(const TSoftObjectPtr<UObject> Asset : BakerySetting->CachedAssets)
    {
        // Not breaking the loop due to a single asset not baking
        BakeCachedAsset(BakerySetting, Asset));
    }

    return BakerySetting->CachedAssets.Num() > 0;
}

bool UMetaDataEditorSubsystem::BakeCachedAsset(const UMetaDataBakingSettingsDataAsset* BakerySetting, const TSoftObjectPtr<UObject>& Asset) const
{

    FMetaScopedBakeSession AssetSession;

    UE_LOG(LogMetaDataEditorSubsystem, Log, TEXT("MetaData Baker : Baking [%s]"), *Asset.ToString());
    
    if (Asset.IsNull())
    {
    
        UE_LOG(LogMetaDataEditorSubsystem, Log, TEXT("MetaData Baker : [%s] is Null"), *Asset.ToString());
        return false;
    }

    if(GEditor->GetEditorSubsystem<UMetaDataIndexerSubsystem>()->GetSoftAssetStatus(Asset.ToSoftObjectPath()) < EMetaDataBakingAssetStatus::MDBAS_Indexed)
    {
        UE_LOG(LogMetaDataEditorSubsystem, Log, TEXT("MetaData Baker : [%s] has not been Indexed"), *Asset.ToString());
        return false;
    }

    // Validate the injected parameter
    if (!IsValid(BakerySetting))
    {
        UE_LOG(LogMetaDataEditorSubsystem, Warning, TEXT("MetadataBaker: Invalid Baking Settings provided for Asset: %s"), *Asset->GetName());
        return false;
    }


    // Extract Traits using the new helper
    TArray<TInstancedStruct<FMetaDataTrait_Base>> ExtractedTraits;
    if (!ExtractAssetTraits(Asset, ExtractedTraits))
    {
        // Extraction failed or had no relevant user data
        
        UE_LOG(LogMetaDataEditorSubsystem, Log, TEXT("MetaData Baker : [%s] Extraction Failed"), *Asset.ToString());
        return false;
    }

    UE_LOG(LogMetaDataEditorSubsystem, Log,
        TEXT("MetaData Baker: [%s] Extracted [%d] Meta Data Entries"),
        *BakerySetting->GetName(),
        ExtractedTraits.Num());

    UMetaDataEditorSubsystem* MetaDataEditorSubsystem = GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>();
    bool bBakeSucceeded = true;
    
    // Stamp the Meta Data Registry Id User Data object if not already on there
    
    IInterface_AssetUserData* Interface_AssetUserData = Cast<IInterface_AssetUserData>(Asset.LoadSynchronous());
    check(Interface_AssetUserData);

    UMetaDataRegistryIdData* MetaDataRegistryIdData = nullptr;
    
    if(Interface_AssetUserData->HasAssetUserDataOfClass(UMetaDataRegistryIdData::StaticClass()))
    {
        MetaDataRegistryIdData = Cast<UMetaDataRegistryIdData>(Interface_AssetUserData->GetAssetUserDataOfClass(UMetaDataRegistryIdData::StaticClass()));
        MetaDataRegistryIdData->AssetRegistryIds.Empty();
    }
    else
    {
        
        MetaDataRegistryIdData = NewObject<UMetaDataRegistryIdData>();
        Interface_AssetUserData->AddAssetUserData(MetaDataRegistryIdData);
    }
    
    
    for (const TInstancedStruct<FMetaDataTrait_Base>& Trait : ExtractedTraits)
    {
        UE_LOG(LogMetaDataEditorSubsystem, Log,
            TEXT("MetaData Baker: Processing [%s] on [%s]"),
            *Trait.GetScriptStruct()->GetFName().ToString(),
            *Asset.ToString());
        
        if (!Trait.IsValid()) 
        {
            continue;
        }

        // Identify the trait type for O(1) routing
        const UScriptStruct* TraitType = Trait.GetScriptStruct(); 

        // Look up the Struct type in your Settings Map
        FMetadataProviderArray TargetGroup = BakerySetting->GetTraitProviders(TraitType);
        UE_LOG(LogMetaDataEditorSubsystem, Display,
            TEXT("MetaData Baker: [%d] Trait Providers"), TargetGroup.ProviderArray.Num());

        
        // Iterate through the array of providers and write
        for (const FMetaDataStorageProviderEntry& Provider : TargetGroup.ProviderArray)
        {
            UE_LOG(LogMetaDataEditorSubsystem, Log,
                TEXT("MetaData Baker: Processing [%s] with [%s]"),
                *Trait.GetScriptStruct()->GetFName().ToString(),
                *GetNameSafe(Provider.Provider));
        
            
            // Ensure the designer selected a naming convention in the settings
            if (!Provider.NamingConvention)
            {
                UE_LOG(LogMetaDataEditorSubsystem, Error, TEXT("MetaData Baker: No Naming Convention configured in Settings Asset [%s]"), *BakerySetting->GetName());
                return false;
            }
            
            // Process Data
            FDataRegistryId RegistryKey = Provider.NamingConvention->GetDataRegistryId(
            IMetaDataStorageProviderInterface::Execute_GetDataRegistryType(Provider.Provider),
            Asset->GetPathName()); 
            
            if (IsValid(Provider.Provider))
            {
                UE_LOG(LogMetaDataEditorSubsystem, Log, TEXT("[%s][%s] Processing [%s]"),
                    *GetNameSafe(this),
                    *GetNameSafe(Provider.Provider),
                    *Asset.ToString());
                // Hand the payload to the resolved provider 
                if (!IMetaDataStorageProviderInterface::Execute_ProcessMetadata(Provider.Provider, RegistryKey, Trait, Asset)) 
                {
                    bBakeSucceeded = false;
                }
                else
                {
                    UE_LOG(LogMetaDataEditorSubsystem, Log, TEXT("[%s] adding [%s] to [%s]"),
                        *GetNameSafe(this),
                        *RegistryKey.ToString(),
                        *Asset.ToString());

                    IMetaDataRegistryDataInterface::Execute_AddRegistryId(MetaDataRegistryIdData, RegistryKey);
                }

                if (MetaDataEditorSubsystem)
                {
                    MetaDataEditorSubsystem->MarkProviderModified(Provider.Provider);
                }
            }

            else
            {
                UE_LOG(LogMetaDataEditorSubsystem, Error,
                    TEXT("MetaData Baker: Failed to bake [%s][%s] -> Invalid Provider"),
                    *Asset.ToString(),
                    *Trait.GetScriptStruct()->GetFName().ToString())
            }
        
        }
    }
    

    FCoreMetaDataEditorDelegates::OnAssetBaked.Broadcast(Asset.ToSoftObjectPath(), bBakeSucceeded);
    return bBakeSucceeded;
}

bool UMetaDataEditorSubsystem::ExtractAssetTraits(const TSoftObjectPtr<UObject>& Asset,
    TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits) const
{
    const IInterface_AssetUserData* MetadataInterface = Cast<IInterface_AssetUserData>(Asset.LoadSynchronous());
    
    // If the saved object is completely unrelated to your trait pipeline, exit cleanly
    if (!MetadataInterface)
    {
        UE_LOG(LogMetaDataEditorSubsystem, Warning,
            TEXT("MetaData Baker : Asset [%s] does not implement Asset User Data interface, cannot extract asset traits"),
            *Asset.ToString());
        return false;
    }


    FMetaDataExtractionResult Result;
    Result.bHasTraits = false;
    Result.bTraitsSuccessfullyExtracted = false;
    
    const TArray<UAssetUserData*>* MetaData = MetadataInterface->GetAssetUserDataArray();
    if (!MetaData || MetaData->Num() == 0)
    {
        UE_LOG(LogMetaDataEditorSubsystem, Warning, TEXT("MetaData Baker : No Asset User Data found on [%s]"), *Asset->GetFName().ToString());
        // Need to broadcast the attempt of the extraction
        FCoreMetaDataEditorDelegates::OnMetaDataExtracted.Broadcast(Asset.ToSoftObjectPath(), Result);
        return false;
    }

    bool bFoundValidData = false;

    for (const UAssetUserData* UserData : *MetaData)
    {
        if (UserData && UserData->Implements<UMetaDataExportInterface>())
        {
            bFoundValidData = true;
            IMetaDataExportInterface::Execute_ExportTraits(UserData, OutTraits, false);
        }
    }

    UE_LOG(LogMetaDataEditorSubsystem, Display, TEXT("MetaData Baker: Exported [%d] Traits"),  OutTraits.Num());

    Result.bHasTraits = OutTraits.Num() > 0;
    Result.bTraitsSuccessfullyExtracted = bFoundValidData;

    FCoreMetaDataEditorDelegates::OnMetaDataExtracted.Broadcast(Asset.ToSoftObjectPath(), Result);
    return bFoundValidData;
}

void UMetaDataEditorSubsystem::BeginBakeSession()
{
    if(_BakeSessionDepth == 0)
    {
        
        UE_LOG(LogTemp, Display, TEXT("Metadata Baker: Bake Session Started"));
        _bBakeCancelled = false;
    }
    
    UE_LOG(LogTemp, Display, TEXT("Metadata Baker: Bake Session Started with depth [%lld]"), _BakeSessionDepth);
    _BakeSessionDepth++;

    
}

void UMetaDataEditorSubsystem::EndBakeSession()
{
    _BakeSessionDepth--;
    UE_LOG(LogTemp, Display, TEXT("Metadata Baker: Bake Session Ended with depth [%lld]"), _BakeSessionDepth);
    if(_BakeSessionDepth == 0)
    {
        UE_LOG(LogTemp, Display, TEXT("Metadata Baker: Bake Session Completed"));
        if(!_bBakeCancelled)
        {
            TSet<TObjectPtr<UObject>> CachedModifiedStorageProviders = ModifiedStorageProviders;
            FlushAllModifiedStorageProviders();

            // Clear the cache from modified providers
            for (UObject* ProviderObj : CachedModifiedStorageProviders)
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
        
                IMetaDataStorageProviderInterface::Execute_ClearCache(ProviderObj);
            }
            
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: Bake session was canceled. Bypassing flush."));
        }
    }
    
}

