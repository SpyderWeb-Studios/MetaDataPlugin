// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataIndexerSubsystem.h"

#include "JsonObjectConverter.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "FunctionLibraries/MetaDataEditorFunctionLibrary.h"
#include "Interfaces/IPluginManager.h"
#include "Widgets/Notifications/SNotificationList.h"

DEFINE_LOG_CATEGORY(LogMetaDataIndexer);

UMetaDataIndexerSubsystem::UMetaDataIndexerSubsystem()
{
    IndexAssetJSONPath = FPaths::ProjectSavedDir() + TEXT("MetadataBaker/AssetIndex.json");
}

void UMetaDataIndexerSubsystem::StartupIndexing(const FAssetData& AssetData)
{
    UE_LOG(LogMetaDataIndexer, Display, TEXT("Asset [%s] Added"), *AssetData.ToSoftObjectPath().ToString());
}


void UMetaDataIndexerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    if (GEditor)
    {
        IAssetRegistry::Get()->OnAssetAdded().AddUObject(this, &UMetaDataIndexerSubsystem::StartupIndexing);
    }
    
}

void UMetaDataIndexerSubsystem::Deinitialize()
{
    bCancelIndexing = true;
    
    SerialiseIndex();
    
    Super::Deinitialize();
}

void UMetaDataIndexerSubsystem::IndexAssets(bool bReIndex)
{
    
	TArray<FDirectoryPath> DirectoriesToBake;
 
	// A. Add the Base Game explicitly
	FDirectoryPath BaseGameDir;
	BaseGameDir.Path = TEXT("/Game");
	DirectoriesToBake.Add(BaseGameDir);
 
	// B. Dynamically discover all DLCs / Mods
	// We only want plugins that actually contain content, ignoring pure-code plugins.
	TArray<TSharedRef<IPlugin>> EnabledPlugins = IPluginManager::Get().GetEnabledPluginsWithContent();
 
	for (const TSharedRef<IPlugin>& Plugin : EnabledPlugins)
	{
		// Optional: If you exclusively want Game Features, you can check Plugin->GetLoadedFrom() 
		// or check if its descriptor type is a GameFeature. 
 
		// Get the exact virtual path string (e.g., "/MyModName")
		FDirectoryPath PluginDir;
		PluginDir.Path = Plugin->GetMountedAssetPath();
 
		DirectoriesToBake.Add(PluginDir);
	}

    IndexAssets_Internal(DirectoriesToBake, bReIndex);
}

void UMetaDataIndexerSubsystem::SerialiseIndex()
{
    FString JsonString;
    UE_LOG(LogMetaDataIndexer, Display, TEXT("Serialising Indexed Assets JSON"));
    if (FJsonObjectConverter::UStructToJsonObjectString(IndexedAssets, JsonString))
    {
    
        UE_LOG(LogMetaDataIndexer, Display, TEXT("Indexed Assets JSON string"));
        if (FFileHelper::SaveStringToFile(JsonString, *IndexAssetJSONPath))
        {
            UE_LOG(LogTemp, Log, TEXT("JSON Struct saved successfully to %s"), *IndexAssetJSONPath);
        }
    }
}

bool UMetaDataIndexerSubsystem::DeserialiseIndex()
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
                else
                {
                    UE_LOG(LogMetaDataIndexer, Warning, TEXT("Could not deserialized asset into IndexedAssets, re-indexing to resolve"));
                    return false;
                }
            }
            UE_LOG(LogMetaDataIndexer, Log, TEXT("Deserialized %d assets into IndexedAssets."), IndexedAssets.IndexedAssets.Num());
        }
        else
        {
            UE_LOG(LogMetaDataIndexer, Warning, TEXT("Asset Array Deserialisation failed, re-indexing to resolve"));
            return false;
        }

        return true;
    }
    return false;
}

void UMetaDataIndexerSubsystem::IndexAssets_Internal(const TArray<FDirectoryPath>& RootFolders, bool bReIndex)
{
    if(bReIndex)
    {
        IndexedAssets.IndexedAssets.Empty();
    }
    
    if (RootFolders.IsEmpty())
    {
        UE_LOG(LogMetaDataIndexer, Error, TEXT("MetadataBaker: Aborting Index. RootFolders array is empty."));
        return;
    }

    if (IndexingHandle.IsValid() && !IndexingHandle.IsReady())
    {
        bCancelIndexing = true;
        IndexingHandle.Wait(); // Wait for the active thread to exit safely
    }
    
    FSlateNotificationManager::Get().CancelProgressNotification(ProgressHandle);

    bCancelIndexing = false;

    
    ProgressHandle.Reset();
    ProgressHandle = FSlateNotificationManager::Get().StartProgressNotification(
        FText::FromString("Meta Data - Indexing Assets"),
        RootFolders.Num()
    );

    UE_LOG(LogMetaDataIndexer, Log, TEXT("MetadataBaker: Indexing Assets from [%d]"), RootFolders.Num());

    
    IndexingHandle = Async(EAsyncExecution::ThreadPool, [this, RootFolders]()
    {
        
        int32 TotalFolders = RootFolders.Num();
        UE_LOG(LogMetaDataIndexer, Warning, TEXT("MetadataBaker: Background Thread Started successfully."));

        for (int i = 0; i < RootFolders.Num(); i++)
        {
            
            if (bCancelIndexing) return; // Immediate exit if Subsystem is shutting down
            
            const FDirectoryPath& Folder = RootFolders[i];
            TArray<FAssetData> FoundAssets;

            UE_LOG(LogMetaDataIndexer, Warning, TEXT("MetadataBaker: Dispatching to Game Thread for Folder: %s"), *Folder.Path);
            // Dispatch to the Game Thread and wait for completion
            FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
            {
                UE_LOG(LogMetaDataIndexer, Warning, TEXT("MetadataBaker: Game Thread Executing Registry Scan."));
                TArray<FAssetData> Assets;
                UMetaDataEditorFunctionLibrary::FindAssetUserDataOwners(Folder, Assets);

                for (const FAssetData& AssetData : Assets)
                {
                    
                    const IInterface_AssetUserData* AssetUserDataObject = Cast<IInterface_AssetUserData>(AssetData.GetAsset());
                    if(!AssetUserDataObject)
                    {
                        UE_LOG(LogMetaDataIndexer, Verbose, TEXT("Asset [%s] cannot be indexed - Does not Implement Asset User Data Interface"), *AssetData.GetObjectPathString());
                        continue;
                    }

                   const TArray<UAssetUserData*>* AssetUserDataArray = AssetUserDataObject->GetAssetUserDataArray();
                    if(!AssetUserDataArray)
                    {
                        UE_LOG(LogMetaDataIndexer, Verbose, TEXT("Asset [%s] cannot be indexed - Asset User Data Array is Invalid"), *AssetData.GetObjectPathString());
                        continue;
                    }

                    if(AssetUserDataArray->Num() == 0)
                    {
                        UE_LOG(LogMetaDataIndexer, Verbose, TEXT("Asset [%s] cannot be indexed - Asset User Data Array is Empty"), *AssetData.GetObjectPathString());
                        continue;
                    }

                    FoundAssets.Add(AssetData);
                }
                
            },
            TStatId(), nullptr, ENamedThreads::GameThread);
        
            // Block this background thread until the Game Thread finishes scanning
            Task->Wait();
            
            UE_LOG(LogMetaDataIndexer, Warning, TEXT("MetadataBaker: Game Thread Finished. Found %d assets."), FoundAssets.Num());
        
            // Process data on the Background Thread safely
            for (const FAssetData& AssetData : FoundAssets)
            {                
                if (bCancelIndexing) return; // Immediate exit if Subsystem is shutting down
                
                // Lock the mutex before writing to the shared array
                FScopeLock Lock(&IndexingMutex);
                IndexedAssets.IndexedAssets.Add(AssetData.GetSoftObjectPath());
                UE_LOG(LogMetaDataIndexer, VeryVerbose, TEXT("MetadataBaker: Adding [%s] to Index"), *AssetData.GetSoftObjectPath().ToString());
            }

            FPlatformProcess::Sleep(0.05f);
            
            // Update progress bar on Game Thread (Fire and forget is safe here)
            AsyncTask(ENamedThreads::GameThread, [this, i, TotalFolders]() {
                if (ProgressHandle.IsValid())
                {
                    FSlateNotificationManager::Get().UpdateProgressNotification(ProgressHandle, i + 1);
                }
            });
        }

        AsyncTask(ENamedThreads::GameThread, [this]() {
            if (ProgressHandle.IsValid())
            {
                FSlateNotificationManager::Get().CancelProgressNotification(ProgressHandle);


                FNotificationInfo Completion(FText::FromString("Meta Data Indexing Complete"));
                Completion.FadeOutDuration = 2.0f;
                Completion.ExpireDuration = 2.0f;
                Completion.bFireAndForget = true;
                FSlateNotificationManager::Get().AddNotification(Completion);
            }
        });
        
    });
}

