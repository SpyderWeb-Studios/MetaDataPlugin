// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/AssetUserData/MetaDataRegistryIdData.h"

void UMetaDataRegistryIdData::AddRegistryId(const FDataRegistryId& RegistryKey)
{
	AssetRegistryIds.Add(RegistryKey);
}

void UMetaDataRegistryIdData::SetRegistryIds(const TSet<FDataRegistryId>& RegistryIds)
{
	AssetRegistryIds = RegistryIds;
}
