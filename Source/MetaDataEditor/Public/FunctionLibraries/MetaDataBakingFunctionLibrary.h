// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MetaDataBakingFunctionLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMetaDataBakingLibrary, Log, Log)

/**
 * 
 */
UCLASS()
class METADATAEDITOR_API UMetaDataBakingFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool BakeMetadataForAsset(UObject* Asset);

	UFUNCTION()
	static void ProcessBakeForDirectory(const FDirectoryPath& Directory);
	
	UFUNCTION(BlueprintCallable)
	static bool ScanForBakingSettings(const FDirectoryPath& RootFolder, TMap<UMetaDataBakingSettingsDataAsset*, FName>& OutBakingSettings);
		
	static UMetaDataBakingSettingsDataAsset* ResolveSettingsForAsset(const FString& TargetAssetPath,
																 const TMap<UMetaDataBakingSettingsDataAsset*, FName>&
																 SettingsMap);
	
};
