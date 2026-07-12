// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Interfaces/MetaDataStorageProviderInterface.h"
#include "MetaDataStorageProvider_Base.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMetaDataStorageProvider, Log, Log)

/**
 * 
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew, Blueprintable, BlueprintType)
class METADATAEDITORCORE_API UMetaDataStorageProvider_Base : public UObject, public IMetaDataStorageProviderInterface
{
	GENERATED_BODY()
	
	virtual bool ValidateTarget_Implementation(const UScriptStruct* TraitType) const override;

	/**
	 * Takes the deterministic key and the pure data payload.
	 * Passed as a const reference to ensure providers remain stateless.
	 */
	
	virtual bool ProcessMetadata_Implementation(const FName& RegistryKey, const TInstancedStruct<FMetaDataTrait_Base>& Payload, const TSoftObjectPtr<UObject>& UnderlyingAsset) override;

	/**
	 * Called after all traits are processed. Used to call MarkPackageDirty(),
	 * save UDataAssets to disk, or close external file streams.
	 */
	
	virtual void Flush_Implementation() override;

};

