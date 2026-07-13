// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/AssetUserData/MetaDataRegistryIdData.h"

void UMetaDataRegistryIdData::SetRegistryIds_Implementation(const TSet<FDataRegistryId>& RegistryIds)
{
	AssetRegistryIds = RegistryIds;
}

void UMetaDataRegistryIdData::AddRegistryId_Implementation(const FDataRegistryId& RegistryKey)
{
	AssetRegistryIds.Add(RegistryKey);
}

void UMetaDataRegistryIdData::GetAllRegistryIds_Implementation(TSet<FDataRegistryId>& OutRegistryIds) const
{
	OutRegistryIds = AssetRegistryIds;
}

bool UMetaDataRegistryIdData::GetRegistryIdForType_Implementation(const FDataRegistryType& RegistryType,
	TSet<FDataRegistryId>& OutRegistryIds) const
{
	OutRegistryIds.Empty(AssetRegistryIds.Num());
	for(const FDataRegistryId& Id : AssetRegistryIds)
	{
		if(Id.RegistryType == RegistryType)
		{
			OutRegistryIds.Add(Id);
		}
	}

	return OutRegistryIds.Num() > 0;
}

