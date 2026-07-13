// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataRegistryId.h"
#include "Engine/AssetUserData.h"
#include "Interfaces/MetaDataRegistryDataInterface.h"
#include "MetaDataRegistryIdData.generated.h"

/**
 * 
 */
UCLASS()
class METADATAPLUGIN_API UMetaDataRegistryIdData : public UAssetUserData, public IMetaDataRegistryDataInterface
{
	GENERATED_BODY()

	friend class UMetaDataEditorSubsystem;
	
public:

	virtual void AddRegistryId_Implementation(const FDataRegistryId& RegistryKey) override;
	virtual void GetAllRegistryIds_Implementation(TSet<FDataRegistryId>& OutRegistryIds) const override;
	virtual bool GetRegistryIdForType_Implementation(const FDataRegistryType& RegistryType,
		TSet<FDataRegistryId>& OutRegistryIds) const override;
	virtual void SetRegistryIds_Implementation(const TSet<FDataRegistryId>& RegistryIds) override;

protected:

	UPROPERTY(VisibleAnywhere)
	TSet<FDataRegistryId> AssetRegistryIds;

};
