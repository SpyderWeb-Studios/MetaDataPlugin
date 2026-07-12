// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MetaDataNamingConvention.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class METADATAEDITORCORE_API UMetaDataNamingConvention : public UObject
{
	GENERATED_BODY()

public:
    
	// Generates a deterministic, stable Registry ID based on the asset's path and context.
	UFUNCTION(BlueprintNativeEvent, Category = "Metadata Baking")
	FName GenerateRegistryKey(const FSoftObjectPath& AssetPath) const;

protected:

	UPROPERTY(EditDefaultsOnly)
	FString ContextNamespace;
	
};