// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Objects/MetaDataNamingConvention.h"
#include "MetaDataBakingSettingsDataAsset.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct FMetadataProviderArray
{
	GENERATED_BODY()

};

/**
 * 
 */
UCLASS()
class METADATAEDITOR_API UMetaDataBakingSettingsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	UMetaDataNamingConvention* NamingConvention;
	
	// The central routing map. Links a Trait Struct to its destination Storage Providers.
	UPROPERTY(EditDefaultsOnly, Category = "Metadata Baking|Routing")
	TMap<UScriptStruct*, FMetadataProviderArray> TraitRoutingMap;

	
};
