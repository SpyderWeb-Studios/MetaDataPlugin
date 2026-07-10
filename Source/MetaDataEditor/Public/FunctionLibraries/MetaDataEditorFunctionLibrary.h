// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DataAssets/BakingSettings/MetaDataBakingSettingsDataAsset.h"
#include "MetaDataEditorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class METADATAEDITOR_API UMetaDataEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static bool ScanForBakingSettings(const FDirectoryPath& RootFolder, TMap<UMetaDataBakingSettingsDataAsset*, FName>& OutBakingSettings);
	
	UFUNCTION()
	static void ProcessBakeForDirectory(const FDirectoryPath& Directory);

	static void FindAssetUserDataOwners(const FDirectoryPath& Directory, TArray<FAssetData>& OutAssetData);
};
