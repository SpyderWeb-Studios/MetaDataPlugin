// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Framework/Notifications/NotificationManager.h"
#include "StructUtils/InstancedStruct.h"

#include "MetaDataEditorSubsystem.generated.h"

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
	
public:
	UMetaDataEditorSubsystem();

	void MarkProviderModified(UObject* StorageProvider);

	void FlushAllModifiedStorageProviders();

	// Simple wrapper for the function library with feedback
	void RequestDirectoriesBake(const TArray<FDirectoryPath>& DirectoryPaths);
	
	
protected:

	void BakeDirectory(const FDirectoryPath& Directory);

	bool BakeBakerySetting(UMetaDataBakingSettingsDataAsset* BakerySetting);

	bool BakeCachedAsset(const UMetaDataBakingSettingsDataAsset* BakerySetting, const TSoftObjectPtr<UObject>& Asset);
	
	bool ExtractAssetTraits(const TSoftObjectPtr<>& Asset, TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits);
	
	UPROPERTY(Transient) // Use Transient so it doesn't serialize the reference itself
	TSet<TObjectPtr<UObject>> ModifiedStorageProviders;

private:

	// todo : Scoped Baking Session for automatic flush detection
	int64 _BakeSessionDepth;
	
};
