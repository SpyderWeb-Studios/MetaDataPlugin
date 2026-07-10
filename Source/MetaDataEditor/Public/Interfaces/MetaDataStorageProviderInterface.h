// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InstancedStruct.h"
#include "StructUtils/InstancedStruct.h"
#include "MetaDataStorageProviderInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UMetaDataStorageProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class METADATAEDITOR_API IMetaDataStorageProviderInterface
{
	GENERATED_BODY()

public:

	/** * 
	 * Checks if the target destination (e.g., Data Table) exists and matches the struct schema.
	 */
	UFUNCTION(BlueprintNativeEvent)
	 bool ValidateTarget(const UScriptStruct* TraitType) const;

	/** 
	 * Takes the deterministic key and the pure data payload. 
	 * Passed as a const reference to ensure providers remain stateless.
	 */
	 UFUNCTION(BlueprintNativeEvent)
	 bool ProcessMetadata(const FName& RegistryKey, const FInstancedStruct& Payload);

	/**
	 * Called after all traits are processed. Used to call MarkPackageDirty(), 
	 * save UDataAssets to disk, or close external file streams.
	 */
	 UFUNCTION(BlueprintNativeEvent)
	 void Flush();
};