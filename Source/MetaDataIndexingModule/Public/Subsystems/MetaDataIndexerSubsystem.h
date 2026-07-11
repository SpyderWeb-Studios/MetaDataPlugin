// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Libraries/FAssetIndexer.h"
#include "MetaDataIndexerSubsystem.generated.h"

/**
 * This is designed to isolate the indexing logic to reduce the bloat in the core meta data editor subsystem.
 *
 * This includes:
 *  - JSON De/Serialisation
 *  - Data Registry Data Stamping on assets
 *    - Source Control Management
 */

DECLARE_LOG_CATEGORY_EXTERN(LogMetaDataIndexer, Log, Log);

UCLASS()
class METADATAINDEXINGMODULE_API UMetaDataIndexerSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	UMetaDataIndexerSubsystem();
	
	void StartupIndexing(const FAssetData& AssetData);
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void IndexAssets(bool bReIndex = false);

	UFUNCTION(BlueprintCallable)
	void SerialiseIndex();

	UFUNCTION()
	bool DeserialiseIndex();

protected:

	void IndexAssets_Internal(const TArray<FDirectoryPath>& RootFolder, bool bReIndex = false);

	
	FString IndexAssetJSONPath;

	FAssetIndexer IndexedAssets;
	FCriticalSection IndexingMutex;
	std::atomic<bool> bCancelIndexing{false};
	FProgressNotificationHandle ProgressHandle;
	TFuture<void> IndexingHandle;
	
};
