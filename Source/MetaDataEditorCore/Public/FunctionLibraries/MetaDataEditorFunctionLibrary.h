// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MetaDataEditorFunctionLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMetaDataEditorCore, Log, Log)

/**
 * 
 */
UCLASS()
class METADATAEDITORCORE_API UMetaDataEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	static void FindAssetUserDataOwners(const FDirectoryPath& Directory, TArray<FAssetData>& OutAssetData);


};
