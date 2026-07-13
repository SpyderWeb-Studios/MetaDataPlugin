// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "DataRegistryId.h"
#include "DataRegistryTypes.h"

#include "MetaDataRegistryDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMetaDataRegistryDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class METADATAPLUGIN_API IMetaDataRegistryDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void AddRegistryId(const FDataRegistryId& RegistryKey);

	UFUNCTION(BlueprintNativeEvent)
	void SetRegistryIds(const TSet<FDataRegistryId>& RegistryIds);

	UFUNCTION(BlueprintNativeEvent)
	bool GetRegistryIdForType(const FDataRegistryType& RegistryType, TSet<FDataRegistryId>& OutRegistryIds) const;

	UFUNCTION(BlueprintNativeEvent)
	void GetAllRegistryIds(TSet<FDataRegistryId>& OutRegistryIds) const;
	
	
};
