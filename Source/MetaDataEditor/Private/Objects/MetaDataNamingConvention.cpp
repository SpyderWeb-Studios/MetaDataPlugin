// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MetaDataNamingConvention.h"

FName UMetaDataNamingConvention::GenerateRegistryKey_Implementation(const FSoftObjectPath& AssetPath,
	const FString& ContextNamespace) const
{
	// Example: FName("MB_" + FMD5::HashAnsiString(AssetPath.ToString()).Left(12))
	FString HashString = FMD5::HashAnsiString(*AssetPath.ToString()).Left(12);
	FString FinalKey = FString::Printf(TEXT("%s_%s"), *ContextNamespace, *HashString);
    
	return FName(*FinalKey);
}

