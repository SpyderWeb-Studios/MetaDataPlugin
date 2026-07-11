// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MetaDataNamingConvention.h"

FName UMetaDataNamingConvention::GenerateRegistryKey_Implementation(const FSoftObjectPath& AssetPath) const
{
	FString PackagePath = AssetPath.GetLongPackageName();

	// Remove the leading slash if it exists (e.g., /Game/ -> Game/)
	if (PackagePath.StartsWith(TEXT("/")))
	{
		PackagePath.RightChopInline(1);
	}
	
	PackagePath.ReplaceCharInline(TEXT('/'), TEXT('_'));

	// Combine with your ContextNamespace
	FString FinalKey = FString::Printf(TEXT("%s_%s"), *ContextNamespace, *PackagePath);

	return FName(*FinalKey);
}
