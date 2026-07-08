// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/BakingSettings/MetaDataBakingSettingsDataAsset.h"

FPrimaryAssetId UMetaDataBakingSettingsDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("MetaDataBakingSettings", GetFName());
}
