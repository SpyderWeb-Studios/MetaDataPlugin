// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataRegistryId.h"

#include "FMetaDataTrait_Base.generated.h"

/**
 * Struct that holds the basic meta data about an asset
 */
USTRUCT(BlueprintType)
struct FMetaDataTrait_Base : public FTableRowBase
{
	GENERATED_BODY()
	
};


FString static GetMetaDataTraitName(const FMetaDataTrait_Base& Trait)
{
	// Use the static method provided by the USTRUCT macro
	return FMetaDataTrait_Base::StaticStruct()->GetName();
}