// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Objects/MetaDataNamingConvention.h"
#include <Objects/Providers/MetaDataStorageProvider_Base.h>
#include "MetaDataBakingSettingsDataAsset.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct FMetadataProviderArray
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UMetaDataStorageProvider_Base*> ProviderArray;

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

	UFUNCTION(BlueprintCallable)
	UMetaDataNamingConvention* GetNamingConvention() const {return NamingConvention;}

	UFUNCTION(BlueprintCallable)
	FMetadataProviderArray GetTraitProviders(const UScriptStruct* Struct) const {return TraitRoutingMap.FindRef(Struct);}
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	UMetaDataNamingConvention* NamingConvention;
	
	// The central routing map. Links a Trait Struct to its destination Storage Providers.
	UPROPERTY(EditDefaultsOnly, Category = "Metadata Baking|Routing")
	TMap<UScriptStruct*, FMetadataProviderArray> TraitRoutingMap;

	
};
