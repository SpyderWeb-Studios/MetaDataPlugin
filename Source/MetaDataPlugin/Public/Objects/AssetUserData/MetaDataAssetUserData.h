// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetUserData.h"
#include "Interfaces/MetaDataExportInterface.h"
#include "MetaDataAssetUserData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class METADATAPLUGIN_API UMetaDataAssetUserData : public UAssetUserData, public IMetaDataExportInterface
{
	GENERATED_BODY()

public:

	UMetaDataAssetUserData();
	
	virtual void ExportTraits_Implementation(TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits,
		bool bClearArray) const override;
	
	virtual bool IsEditorOnly() const override;

protected:

	UPROPERTY(EditDefaultsOnly)
	TArray<TInstancedStruct<FMetaDataTrait_Base>> MetaData;

	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	bool bEditorOnly = true;
};
