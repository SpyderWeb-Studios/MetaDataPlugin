// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataRegistryId.h"
#include "Engine/AssetUserData.h"
#include "MetaDataRegistryIdData.generated.h"

/**
 * 
 */
UCLASS()
class METADATAPLUGIN_API UMetaDataRegistryIdData : public UAssetUserData
{
	GENERATED_BODY()

	friend class UMetaDataEditorSubsystem;
	
public:

	void AddRegistryId(const FDataRegistryId& RegistryKey);

	void SetRegistryIds(const TSet<FDataRegistryId>& RegistryIds);

protected:

	UPROPERTY(VisibleAnywhere)
	TSet<FDataRegistryId> AssetRegistryIds;

};
