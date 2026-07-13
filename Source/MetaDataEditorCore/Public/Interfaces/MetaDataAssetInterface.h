// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MetaDataAssetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMetaDataAssetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class METADATAEDITORCORE_API IMetaDataAssetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void RefreshMetaDataAsset(UObject* OwningAsset);

};
