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

FDataRegistryId UMetaDataNamingConvention::GetDataRegistryId_Implementation(const FDataRegistryType& TargetRegistryType, const FSoftObjectPath& AssetPath) const
{
	// Get deterministic FName (The Row Name) using the function
	FName ItemName = GenerateRegistryKey(AssetPath);

	// Validate that a Registry Type has been assigned to prevent silent failures
	if (!TargetRegistryType.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("MetaDataNamingConvention: TargetRegistryType is invalid! Ensure you have set a Registry Type in your Naming Convention asset for %s"), *AssetPath.ToString());
	}

	// Combine them into a standardized Data Registry ID
	return FDataRegistryId(TargetRegistryType, ItemName);
}
