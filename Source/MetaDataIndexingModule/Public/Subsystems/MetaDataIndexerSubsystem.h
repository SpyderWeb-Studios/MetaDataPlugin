// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "UObject/AssetRegistryTagsContext.h"

#include "Libraries/FExternalBakingSetting.h"
#include "Libraries/FBakingSettingsAssetIndexer.h"
#include "Libraries/FExtractionResult.h"

#include "Framework/Notifications/NotificationManager.h"
#include "MetaDataIndexerSubsystem.generated.h"

/**
 * This is designed to isolate the indexing logic to reduce the bloat in the core meta data editor subsystem.
 *
 * This includes:
 *  - Data Registry Data Stamping on assets
 *    - Source Control Management
 */

class UMetaDataBakingSettingsDataAsset;
DECLARE_LOG_CATEGORY_EXTERN(LogMetaDataIndexer, Log, Log);

UCLASS()
class METADATAINDEXINGMODULE_API UMetaDataIndexerSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

	friend class UMetaDataEditorSubsystem;
	
public:
	UMetaDataIndexerSubsystem();


	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	

	void RefreshIndex();

	void GetDirectoryIndex(const FDirectoryPath& DirectoryPath, TSet<TSoftObjectPtr<UMetaDataBakingSettingsDataAsset>>& OutIndex);

	EMetaDataBakingAssetStatus GetSoftAssetStatus(const FSoftObjectPath& SoftAsset) const;

	TSoftObjectPtr<UMetaDataBakingSettingsDataAsset> GetBakingSettingForAsset(const FSoftObjectPath& SoftAsset);
	
	void RefreshDataAssetCache(UMetaDataBakingSettingsDataAsset* DataAsset);
protected:


	void SerialiseIndex();
	void DeserialiseIndex();
	
	
	void HandleAssetBaked( const FSoftObjectPath&, bool bSuccess);
	void HandleMetaDataExtracted( const FSoftObjectPath&, FMetaDataExtractionResult MetaDataExtractionResult);


	void UpdateAssetIndexStatus(const FSoftObjectPath& Asset, EMetaDataBakingAssetStatus Status);
	
	TSet<TSoftObjectPtr<UMetaDataBakingSettingsDataAsset>> CachedBakingSettingsDataAssets;

	TSet<FMetaDataBakingSettingsAssetIndex> CachedIndex;

private:

	FString CacheFile;
	
};
