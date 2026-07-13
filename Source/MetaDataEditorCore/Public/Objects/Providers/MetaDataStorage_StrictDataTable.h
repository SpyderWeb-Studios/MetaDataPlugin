// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Providers/MetaDataStorageProvider_Base.h"
#include "MetaDataStorage_StrictDataTable.generated.h"

/**
 * 
 */
UCLASS()
class METADATAEDITORCORE_API UMetaDataStorage_StrictDataTable : public UMetaDataStorageProvider_Base
{
	GENERATED_BODY()
public:

	virtual bool ValidateTarget_Implementation(const UScriptStruct* TraitType) const override;
	virtual void Flush_Implementation() override;
	
	virtual void ClearCache_Implementation() override;
	
	virtual bool ProcessMetadata_Implementation(const FDataRegistryId& RegistryKey,
		const TInstancedStruct<FMetaDataTrait_Base>& Payload, const TSoftObjectPtr<>& UnderlyingAsset) override;
	
	virtual FDataRegistryType GetDataRegistryType_Implementation() const override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Storage")
	TSoftObjectPtr<UDataTable> TargetDataTable;

	// Cache the extracted instanced structs during the loop
	UPROPERTY(VisibleAnywhere, Category = "Storage")
	TMap<FName, TInstancedStruct<FMetaDataTrait_Base>> CachedTraits;

	UPROPERTY(EditAnywhere, Category = "Storage")
	FDataRegistryType DataRegistryType;
	
	
};
