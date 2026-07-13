// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Libraries/Runtime/FAssetMetaDataIdSet.h"
#include "MetaDataRuntimeSubsystem.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogMetaDataRuntime, Log, Log)

/**
 * 
 */
UCLASS()
class METADATAPLUGIN_API UMetaDataRuntimeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	bool GetAssetMetaDataSet(UObject* Asset, FAssetMetaDataIdSet& OutSet);

	bool GetAssetMetaDataSetForType(UObject* Asset, const FDataRegistryType& DataRegistryType, TSet<FDataRegistryId>& OutSet);
	
	
	protected:

	void CacheInvalidation();
	
	UPROPERTY(Transient)
	TMap<TWeakObjectPtr<UObject>, FAssetMetaDataIdSet> AssetIdCache;
	
	
};
