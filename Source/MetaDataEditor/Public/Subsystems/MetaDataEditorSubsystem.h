// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "MetaDataEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class METADATAEDITOR_API UMetaDataEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void ExtractAndBakeMetadata(UObject* MeshAsset);

	void GetModifiedMetadataTables(TSet<UDataTable*>& OutTables) const {OutTables = ModifiedTables;}

protected:
	
	TArray<UDataTable*> GetAllMetadataTables();
	
	// The delegate hook
	void OnPreSavePackage(UPackage* Package, FObjectPreSaveContext Context);
	
	void OnAssetDeleted(const FAssetData& AssetData);

	UPROPERTY(BlueprintReadOnly)
	TSet<UDataTable*> ModifiedTables;

};
