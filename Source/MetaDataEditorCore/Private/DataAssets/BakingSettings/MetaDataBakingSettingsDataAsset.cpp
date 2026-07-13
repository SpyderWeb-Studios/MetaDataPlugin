// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/BakingSettings/MetaDataBakingSettingsDataAsset.h"

#include "AssetRegistry/AssetRegistryModule.h"

DEFINE_LOG_CATEGORY(LogMetaDataSettings);

FPrimaryAssetId UMetaDataBakingSettingsDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("MetaDataBakingSettings", GetFName());
}

void UMetaDataBakingSettingsDataAsset::AssignCache(const TSet<TSoftObjectPtr<UObject>>& Cache)
{
	CachedAssets = Cache;
}

bool UMetaDataBakingSettingsDataAsset::IsBakingAssetAutoScanning() const
{
return ExternalBakingSetting.bAutoScan;
}

FExternalBakingSetting* UMetaDataBakingSettingsDataAsset::UpdateBakingAssetSetting()
{
	return &ExternalBakingSetting;
}


