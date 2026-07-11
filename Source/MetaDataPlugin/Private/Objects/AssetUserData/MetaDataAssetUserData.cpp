// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/AssetUserData/MetaDataAssetUserData.h"


void UMetaDataAssetUserData::ExportTraits_Implementation(TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits,
                                                         bool bClearArray) const
{
	if(bClearArray)
	{
		OutTraits.Empty();
	}
	OutTraits.Append(MetaData);
}
