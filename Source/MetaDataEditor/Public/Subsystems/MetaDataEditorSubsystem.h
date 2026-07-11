// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Framework/Notifications/NotificationManager.h"
#include "MetaDataEditorSubsystem.generated.h"

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

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void MarkProviderModified(UObject* StorageProvider);

	void FlushAllModifiedStorageProviders();

	// Simple wrapper for the function library with feedback
	void RequestDirectoriesBake(const TArray<FDirectoryPath>& DirectoryPaths);
	
protected:
	
	TArray<UDataTable*> GetAllMetadataTables();
	
	// The delegate hook
	void OnPreSavePackage(UPackage* Package, FObjectPreSaveContext Context);
	
	void OnAssetDeleted(const FAssetData& AssetData);
	
	UPROPERTY(Transient) // Use Transient so it doesn't serialize the reference itself
	TSet<TObjectPtr<UObject>> ModifiedStorageProviders;

};
