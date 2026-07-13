// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Factories/Factory.h"
#include "Framework/Notifications/NotificationManager.h"
#include "StructUtils/InstancedStruct.h"

#include "MetaDataEditorSubsystem.generated.h"

class UMetaDataIndexerSubsystem;
struct FMetaDataTrait_Base;
class UMetaDataBakingSettingsDataAsset;
class UMetaDataStorageProviderInterface;

DECLARE_LOG_CATEGORY_EXTERN(LogMetaDataEditorSubsystem, Log, Log);


/**
 * 
 */
UCLASS()
class METADATAEDITOR_API UMetaDataEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	friend struct FMetaScopedBakeSession;
public:
	UMetaDataEditorSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void MarkProviderModified(UObject* StorageProvider);

	void FlushAllModifiedStorageProviders();

	// Simple wrapper for the function library with feedback
	void RequestDirectoriesBake(const TArray<FDirectoryPath>& DirectoryPaths);

	void RequestBakerySettingBake(UMetaDataBakingSettingsDataAsset* DataAssetPath) const;

	void RequestAssetBake(const UMetaDataBakingSettingsDataAsset* BakerySetting, const FSoftObjectPath& Asset) const; 

protected:
	bool BakeDirectory(const FDirectoryPath& Directory) const;

	bool BakeBakerySetting(UMetaDataBakingSettingsDataAsset* BakerySetting) const;

	bool BakeCachedAsset(const UMetaDataBakingSettingsDataAsset* BakerySetting, const TSoftObjectPtr<UObject>& Asset) const;
	
	bool ExtractAssetTraits(const TSoftObjectPtr<>& Asset, TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits) const;
	
	UPROPERTY(Transient) // Use Transient so it doesn't serialize the reference itself
	TSet<TObjectPtr<UObject>> ModifiedStorageProviders;

/* Callback Handlers */
	
	void HandleAssetRenamed(const FAssetData& AssetData, const FString& String);
	void HandlePostImport(UFactory* Factory, UObject* Object);
	void HandlePostReImport(UObject* Object);
	void HandleObjectModified(UObject* Object);
	void HandleObjectPreSave(UObject* Object, FObjectPreSaveContext ObjectPreSaveContext);
	void HandleAssetRemoved(const FAssetData& AssetData);
	
	
private:

	// todo : Scoped Baking Session for automatic flush detection
	int64 _BakeSessionDepth;
	bool _bBakeCancelled;

	void BeginBakeSession();
	void EndBakeSession();
	void RefreshAsset(UObject* Asset);
};
