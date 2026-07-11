// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataEditorSubsystem.h"
#include "InstancedStruct.h"
#include "JsonObjectConverter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Framework/Notifications/NotificationManager.h"
#include "FunctionLibraries/MetaDataEditorFunctionLibrary.h"
#include "Interfaces/IPluginManager.h"
#include "UObject/ObjectSaveContext.h"

#include "Libraries/FMetaDataRegistryItem.h"
#include "Widgets/Notifications/SNotificationList.h"


UMetaDataEditorSubsystem::UMetaDataEditorSubsystem()
{
    IndexAssetJSONPath = FPaths::ProjectSavedDir() + TEXT("MetadataBaker/AssetIndex.json");
}

void UMetaDataEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (GEditor)
    {
        FEditorDelegates::OnEditorInitialized.AddUObject(this, &UMetaDataEditorSubsystem::StartupIndexing);
    }

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

    bIsStopping = true;
    SerialiseIndexAssets();
    
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
            UE_LOG(LogMetaDataBakerEditor, Warning, TEXT("Modified Provider [%s] is not valid"), *GetNameSafe(ProviderObj));
            continue;
        }

        if(!ProviderObj->Implements<UMetaDataStorageProviderInterface>())
        {
            UE_LOG(LogMetaDataBakerEditor, Warning, TEXT("Modified Provider [%s] does not implement IMetaDataStorageProviderInterface"), *GetNameSafe(ProviderObj));
            continue;
        }
        
        IMetaDataStorageProviderInterface::Execute_Flush(ProviderObj);
    }
    
    // Clear the set after processing
    ModifiedStorageProviders.Empty();
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
            UE_LOG(LogTemp, Display, TEXT("Asset Deleted: Cleaned up row [%s] from [%s]"), *RowNameToRemove.ToString(), *Table->GetName());
        }
    }
}

void UMetaDataEditorSubsystem::StartupIndexing(double Duration)
{
    // Indexer
    FString Assets_JSON;
    FFileHelper::LoadFileToString(Assets_JSON, *IndexAssetJSONPath);
    TSharedPtr<FJsonObject> Assets_JsonObject = MakeShareable(new FJsonObject());
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Assets_JSON);

    if (FJsonSerializer::Deserialize(JsonReader, Assets_JsonObject) && Assets_JsonObject.IsValid())
    {
        // 1. Get the array from the JSON
        const TArray<TSharedPtr<FJsonValue>>* JsonArray;
        if (Assets_JsonObject->TryGetArrayField(TEXT("IndexedAssets"), JsonArray))
        {
            IndexedAssets.IndexedAssets.Empty(); // Clear existing
            
            for (const TSharedPtr<FJsonValue>& Value : *JsonArray)
            {
                FString AssetPath;
                if (Value->TryGetString(AssetPath))
                {
                    FSoftObjectPath ObjectPath(AssetPath);
                    IndexedAssets.IndexedAssets.Add(ObjectPath);
                }
            }
            UE_LOG(LogTemp, Log, TEXT("Deserialized %d assets into IndexedAssets."), IndexedAssets.IndexedAssets.Num());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Couldn't deserialize AssetIndex.json, performing scan in background"));

        TArray<FDirectoryPath> DirectoriesToBake;

        // A. Add the Base Game explicitly
        FDirectoryPath BaseGameDir;
        BaseGameDir.Path = TEXT("/Game");
        DirectoriesToBake.Add(BaseGameDir);

        // We only want plugins that actually contain content, ignoring pure-code plugins.
        TArray<TSharedRef<IPlugin>> EnabledPlugins = IPluginManager::Get().GetEnabledPluginsWithContent();

        for (const TSharedRef<IPlugin>& Plugin : EnabledPlugins)
        {
            FDirectoryPath PluginDir;
            PluginDir.Path = Plugin->GetMountedAssetPath();

            DirectoriesToBake.Add(PluginDir);
        }

        GEditor->GetTimerManager()->SetTimerForNextTick([this, DirectoriesToBake]
        {
            IndexAssets(DirectoriesToBake);        
        });
        
    }
}

void UMetaDataEditorSubsystem::IndexAssets(const TArray<FDirectoryPath>& RootFolders)
{
    IndexedAssets.IndexedAssets.Empty();
    
    if (RootFolders.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("MetadataBaker: Aborting Index. RootFolders array is empty."));
        return;
    }

    if (IndexingHandle.IsValid() && !IndexingHandle.IsReady())
    {
        bIsStopping = true;
        IndexingHandle.Wait(); // Wait for the active thread to exit safely
    }
    
    FSlateNotificationManager::Get().CancelProgressNotification(ProgressHandle);

    bIsStopping = false;

    
    ProgressHandle.Reset();
    ProgressHandle = FSlateNotificationManager::Get().StartProgressNotification(
        FText::FromString("Meta Data - Indexing Assets"),
        RootFolders.Num()
    );

    UE_LOG(LogMetaDataBakerEditor, Log, TEXT("MetadataBaker: Indexing Assets from [%d]"), RootFolders.Num());

    
    IndexingHandle = Async(EAsyncExecution::ThreadPool, [this, RootFolders]()
    {
        
        int32 TotalFolders = RootFolders.Num();
        UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: Background Thread Started successfully."));

        for (int i = 0; i < RootFolders.Num(); i++)
        {
            
            if (bIsStopping) return; // Immediate exit if Subsystem is shutting down
            
            const FDirectoryPath& Folder = RootFolders[i];
            TArray<FAssetData> FoundAssets;

            UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: Dispatching to Game Thread for Folder: %s"), *Folder.Path);
            // Dispatch to the Game Thread and wait for completion
            FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
            {
                UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: Game Thread Executing Registry Scan."));
                TArray<FAssetData> Assets;
                UMetaDataEditorFunctionLibrary::FindAssetUserDataOwners(Folder, Assets);

                for (const FAssetData& AssetData : Assets)
                {
                    
                    const IInterface_AssetUserData* AssetUserDataObject = Cast<IInterface_AssetUserData>(AssetData.GetAsset());
                    if(!AssetUserDataObject)
                    {
                        UE_LOG(LogMetaDataBakerEditor, Verbose, TEXT("Asset [%s] cannot be indexed - Does not Implement Asset User Data Interface"), *AssetData.GetObjectPathString());
                        continue;
                    }

                   const TArray<UAssetUserData*>* AssetUserDataArray = AssetUserDataObject->GetAssetUserDataArray();
                    if(!AssetUserDataArray)
                    {
                        UE_LOG(LogMetaDataBakerEditor, Verbose, TEXT("Asset [%s] cannot be indexed - Asset User Data Array is Invalid"), *AssetData.GetObjectPathString());
                        continue;
                    }

                    if(AssetUserDataArray->Num() == 0)
                    {
                        UE_LOG(LogMetaDataBakerEditor, Verbose, TEXT("Asset [%s] cannot be indexed - Asset User Data Array is Empty"), *AssetData.GetObjectPathString());
                        continue;
                    }

                    FoundAssets.Add(AssetData);
                }
                
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
        
            // Block this background thread until the Game Thread finishes scanning
            Task->Wait();
            
            UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: Game Thread Finished. Found %d assets."), FoundAssets.Num());
        
            // Process data on the Background Thread safely
            for (const FAssetData& AssetData : FoundAssets)
            {                
                if (bIsStopping) return; // Immediate exit if Subsystem is shutting down
                
                // Lock the mutex before writing to the shared array
                FScopeLock Lock(&IndexingMutex);
                IndexedAssets.IndexedAssets.Add(AssetData.GetSoftObjectPath());
                UE_LOG(LogTemp, VeryVerbose, TEXT("MetadataBaker: Adding [%s] to Index"), *AssetData.GetSoftObjectPath().ToString());
            }

            FPlatformProcess::Sleep(0.5f);
            
            // Update progress bar on Game Thread (Fire and forget is safe here)
            AsyncTask(ENamedThreads::GameThread, [this, i, TotalFolders]() {
                if (ProgressHandle.IsValid())
                {
                    FSlateNotificationManager::Get().UpdateProgressNotification(ProgressHandle, i + 1);
                }
            });
        }

        // 3. Finalization (Game Thread)
        AsyncTask(ENamedThreads::GameThread, [this]() {
            if (ProgressHandle.IsValid())
            {
                FSlateNotificationManager::Get().CancelProgressNotification(ProgressHandle);


                FNotificationInfo Completion(FText::FromString("Meta Data Indexing Complete"));
                Completion.ExpireDuration = 2.0f;
                Completion.bFireAndForget = true;
                FSlateNotificationManager::Get().AddNotification(Completion);
            }
        });
        
    });

}

void UMetaDataEditorSubsystem::SerialiseIndexAssets() const
{
    // Serialise IndexedAssets
    FString JsonString;
    UE_LOG(LogMetaDataBakerEditor, Display, TEXT("Serialising Indexed Assets JSON"));
    if (FJsonObjectConverter::UStructToJsonObjectString(IndexedAssets, JsonString))
    {
    
        UE_LOG(LogMetaDataBakerEditor, Display, TEXT("Indexed Assets JSON string"));
        if (FFileHelper::SaveStringToFile(JsonString, *IndexAssetJSONPath))
        {
            UE_LOG(LogTemp, Log, TEXT("JSON Struct saved successfully to %s"), *IndexAssetJSONPath);
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
            UMetaDataEditorFunctionLibrary::BakeMetadataForAsset(Obj);
        }
    }

    FlushAllModifiedStorageProviders();
}
