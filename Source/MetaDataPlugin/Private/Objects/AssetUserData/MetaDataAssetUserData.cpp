// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/AssetUserData/MetaDataAssetUserData.h"

#include "Developer/MetaDataBakingSettings.h"
#include "UObject/AssetRegistryTagsContext.h"


UMetaDataAssetUserData::UMetaDataAssetUserData()
{
}

void UMetaDataAssetUserData::ExportTraits_Implementation(TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits,
                                                         bool bClearArray) const
{
	if(bClearArray)
	{
		OutTraits.Empty();
	}
	OutTraits.Append(MetaData);
	
}

bool UMetaDataAssetUserData::IsEditorOnly() const
{
	// Fetch the global settings Default Object (CDO). 
	// This is highly optimized and has zero overhead during the cook phase.
	const UMetaDataBakingSettings* GlobalSettings = GetDefault<UMetaDataBakingSettings>();
    
	if (GlobalSettings)
	{
		bool bGlobalRemove = GlobalSettings->ShouldRemoveMetaDataByDefault();
		bool bForceGlobal = GlobalSettings->ShouldOverrideLocalEditorOnly();
		
		if (bForceGlobal)
		{
			return bGlobalRemove;
		}
#if UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
		// If the global safety net and the local granular toggle conflict.
		if (bGlobalRemove != bEditorOnly)
		{
			// Log a warning so the build log will immediately flag the override.
			// This ensures the team can verify if the bloat was intentional.
			UE_LOG(LogTemp, Warning, TEXT("MetadataBaker: Asset [%s] overrides Global Setting. Local setting is [%s]."), 
				*GetOuter()->GetName(), 
				bEditorOnly ? TEXT("True") : TEXT("False"));
		}
#endif
	}
	
	return bEditorOnly;
}

void UMetaDataAssetUserData::GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const
{
	Super::GetAssetRegistryTags(OutTags);

	// Add your tag
	FAssetRegistryTag NewTag(FName("HasMetaData"), TEXT("True"), FAssetRegistryTag::TT_Alphabetical);
	OutTags.Add(NewTag);
}

void UMetaDataAssetUserData::GetAssetRegistryTags(FAssetRegistryTagsContext Context) const
{

	Super::GetAssetRegistryTags(Context);
	FAssetRegistryTag NewTag(FName("HasMetaData"), TEXT("True"), FAssetRegistryTag::TT_Alphabetical);
	Context.AddTag(NewTag);
}

void UMetaDataAssetUserData::GetAssetRegistryTagMetadata(TMap<FName, FAssetRegistryTagMetadata>& OutMetadata) const
{
	Super::GetAssetRegistryTagMetadata(OutMetadata);
}

