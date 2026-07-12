// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Objects/MetaDataNamingConvention.h"
#include <Objects/Providers/MetaDataStorageProvider_Base.h>

#include "Libraries/FBakingSettingsAssetIndexer.h"
#include "MetaDataBakingSettingsDataAsset.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMetaDataSettings, Log, Log);

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
class METADATAEDITORCORE_API UMetaDataBakingSettingsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION(BlueprintCallable)
	UMetaDataNamingConvention* GetNamingConvention() const {return NamingConvention;}

	UFUNCTION(BlueprintCallable)
	FMetadataProviderArray GetTraitProviders(const UScriptStruct* Struct) const {return TraitRoutingMap.FindRef(Struct);}

	void AssignCache(TSet<TSoftObjectPtr<UObject>> Cache);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSet<TSoftObjectPtr<UObject>> CachedAssets;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Naming")
	UMetaDataNamingConvention* NamingConvention;
	
	// The central routing map. Links a Trait Struct to its destination Storage Providers.
	UPROPERTY(EditDefaultsOnly, Category = "Routing")
	TMap<UScriptStruct*, FMetadataProviderArray> TraitRoutingMap;

	
};
