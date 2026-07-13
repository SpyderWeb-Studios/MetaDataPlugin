// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Providers/MetaDataStorageProvider_Base.h"
#include "Libraries/FMetaDataRegistryItem.h"
#include "MetaDataStorage_DataTable.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced)
class METADATAEDITORCORE_API UMetaDataStorage_DataTable : public UMetaDataStorageProvider_Base
{
	GENERATED_BODY()
	
public: 

	virtual bool ValidateTarget_Implementation(const UScriptStruct* TraitType) const override;

	/**
	 * Takes the deterministic key and the pure data payload.
	 * Passed as a const reference to ensure providers remain stateless.
	 */

	virtual bool ProcessMetadata_Implementation(const FDataRegistryId& RegistryKey, const TInstancedStruct<FMetaDataTrait_Base>& Payload, const TSoftObjectPtr<UObject>& UnderlyingAsset) override;

	/**
	 * Called after all traits are processed. Used to call MarkPackageDirty(),
	 * save UDataAssets to disk, or close external file streams.
	 */

	virtual void Flush_Implementation() override;

	virtual void ClearCache_Implementation() override;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UDataTable> TargetDataTable;

	UPROPERTY(VisibleAnywhere)
	TMap<FDataRegistryId, FMetaDataRegistryItem> CachedRows;

};
