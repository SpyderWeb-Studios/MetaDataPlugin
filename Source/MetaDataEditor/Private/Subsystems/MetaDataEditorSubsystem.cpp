// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataEditorSubsystem.h"
#include "InstancedStruct.h"
#include "JsonObjectConverter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "FunctionLibraries/MetaDataBakingFunctionLibrary.h"
#include "Interfaces/MetaDataStorageProviderInterface.h"
#include "UObject/ObjectSaveContext.h"
#include "Libraries/FMetaDataRegistryItem.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"

DEFINE_LOG_CATEGORY(LogMetaDataEditorSubsystem);

UMetaDataEditorSubsystem::UMetaDataEditorSubsystem()
{
}

void UMetaDataEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);



    // Bind to the asset deletion event
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    AssetRegistryModule.Get().OnAssetRemoved().AddUObject(this, &UMetaDataEditorSubsystem::OnAssetDeleted);

    // 
    UPackage::PreSavePackageWithContextEvent.AddUObject(this, &UMetaDataEditorSubsystem::OnPreSavePackage);
}

void UMetaDataEditorSubsystem::Deinitialize()
{
    if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
    {
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        AssetRegistryModule.Get().OnAssetRemoved().RemoveAll(this);
    }

    UPackage::PreSavePackageWithContextEvent.RemoveAll(this);
    
    Super::Deinitialize();
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

        UMetaDataBakingFunctionLibrary::ProcessBakeForDirectory(Dir);

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

void UMetaDataEditorSubsystem::OnAssetDeleted(const FAssetData& AssetData)
{
    // The RowName is the path of the deleted asset
    FName RowNameToRemove = FName(*AssetData.GetObjectPathString());

    // Check all metadata tables and strip out the deleted asset
    TArray<UDataTable*> AllMetaTables = GetAllMetadataTables();
    for (UDataTable* Table : AllMetaTables)
    {
        if (Table && Table->GetRowMap().Contains(RowNameToRemove))
        {
            Table->RemoveRow(RowNameToRemove);
            Table->MarkPackageDirty();
            UE_LOG(LogMetaDataEditorSubsystem, Display, TEXT("Asset Deleted: Cleaned up row [%s] from [%s]"), *RowNameToRemove.ToString(), *Table->GetName());
        }
    }
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

void UMetaDataEditorSubsystem::OnPreSavePackage(UPackage* Package, FObjectPreSaveContext Context)
{
    // Ignore autosaves or cooking saves to prevent infinite loops
    if (Context.IsProceduralSave() || Context.IsCooking()) return;

    if (!Package) return;

    TArray<UObject*> ObjectsInPackage;
    GetObjectsWithOuter(Package, ObjectsInPackage, false);

    for (UObject* Obj : ObjectsInPackage)
    {
        // 1. Check if the object is either a Static Mesh or a Skeletal Mesh
        if (Obj->IsA<UStaticMesh>() || Obj->IsA<USkeletalMesh>())
        {
            UMetaDataBakingFunctionLibrary::BakeMetadataForAsset(Obj);
        }
    }

    FlushAllModifiedStorageProviders();
}
