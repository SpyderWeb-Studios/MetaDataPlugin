// Fill out your copyright notice in the Description page of Project Settings.


#include "Developer/MetaDataBakingSettings.h"

FName UMetaDataBakingSettings::GetCategoryName() const
{
	return FName("Meta Data Plugin");
}

FText UMetaDataBakingSettings::GetSectionText() const
{
	return NSLOCTEXT("Meta Data Plugin", "Meta Data Plugin", "Baking Settings");
}

FText UMetaDataBakingSettings::GetSectionDescription() const
{
	return Super::GetSectionDescription();
}
