// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "MetaDataBakerCommandlet.generated.h"

/**
 * 
 */
UCLASS()
class METADATAEDITOR_API UMetaDataBakerCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	virtual int32 Main(const FString& Params) override;
	void ScanContentForMeshes(const FDirectoryPath& RootFolder, TArray<FAssetData>& OutAssetData);
	void CommitChanges(TSet<UObject*> ModifiedObjects);

protected:

	
};
