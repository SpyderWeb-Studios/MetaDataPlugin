// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "Libraries/Traits/FMetaDataTrait_Base.h"
#include "UObject/Interface.h"

#include "MetaDataExportInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMetaDataExportInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class METADATAPLUGIN_API IMetaDataExportInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// The automation tool calls this to get the inline objects
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Automation")
	void ExportTraits(TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits, bool bClearArray) const;
	
};
