// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Libraries/AssetIndexer.h"
#include "MetaDataEditorSubsystem.generated.h"

class UMetaDataStorageProviderInterface;



/**
 * 
 */
UCLASS()
class METADATAEDITOR_API UMetaDataEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	UMetaDataEditorSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void MarkProviderModified(UObject* StorageProvider);

	void FlushAllModifiedStorageProviders();

	void IndexAssets(const TArray<FDirectoryPath>& RootFolders);
	void SerialiseIndexAssets() const;
	
protected:
	
	TArray<UDataTable*> GetAllMetadataTables();
	
	// The delegate hook
	void OnPreSavePackage(UPackage* Package, FObjectPreSaveContext Context);
	
	void OnAssetDeleted(const FAssetData& AssetData);

	UFUNCTION()
	void StartupIndexing(double Duration);
	

	
	UPROPERTY(Transient) // Use Transient so it doesn't serialize the reference itself
	TSet<TObjectPtr<UObject>> ModifiedStorageProviders;

	FString IndexAssetJSONPath;

	FAssetIndexer IndexedAssets;
	FCriticalSection IndexingMutex;
	std::atomic<bool> bIsStopping{false};
	FProgressNotificationHandle ProgressHandle;
	TFuture<void> IndexingHandle;
};
