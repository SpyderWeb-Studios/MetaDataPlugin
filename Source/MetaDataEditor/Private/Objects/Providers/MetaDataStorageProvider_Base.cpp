// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Providers/MetaDataStorageProvider_Base.h"

bool UMetaDataStorageProvider_Base::ValidateTarget_Implementation(const UScriptStruct* TraitType) const
{
	return false;
}

bool UMetaDataStorageProvider_Base::ProcessMetadata_Implementation(const FName& RegistryKey, const FInstancedStruct& Payload)
{
	return false;
}

void UMetaDataStorageProvider_Base::Flush_Implementation()
{
}
