// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataRegistryId.h"

#include "FMetaDataTrait_Base.generated.h"

/**
 * Struct that holds the basic meta data about an asset
 */
USTRUCT(BlueprintType)
struct FMetaDataTrait_Base
{
	GENERATED_BODY()
	
	virtual ~FMetaDataTrait_Base() = default;
	
	virtual FString ToString() const;

	UPROPERTY(EditDefaultsOnly, AdvancedDisplay)
	TSoftObjectPtr<UDataTable> TraitRegistry;
	
};

inline FString FMetaDataTrait_Base::ToString() const
{
	return "Meta Data Trait Base";
}
