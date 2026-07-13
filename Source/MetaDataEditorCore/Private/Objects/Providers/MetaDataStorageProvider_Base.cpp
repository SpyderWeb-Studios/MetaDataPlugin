// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Providers/MetaDataStorageProvider_Base.h"


DEFINE_LOG_CATEGORY(LogMetaDataStorageProvider)

void UMetaDataStorageProvider_Base::ClearCache_Implementation()
{
	IMetaDataStorageProviderInterface::ClearCache_Implementation();
}

bool UMetaDataStorageProvider_Base::ValidateTarget_Implementation(const UScriptStruct* TraitType) const
{
	return false;
}

bool UMetaDataStorageProvider_Base::ProcessMetadata_Implementation(const FDataRegistryId& RegistryKey, const TInstancedStruct<FMetaDataTrait_Base>& Payload, const TSoftObjectPtr<UObject>& UnderlyingAsset)
{
	return false;
}

void UMetaDataStorageProvider_Base::Flush_Implementation()
{
}
