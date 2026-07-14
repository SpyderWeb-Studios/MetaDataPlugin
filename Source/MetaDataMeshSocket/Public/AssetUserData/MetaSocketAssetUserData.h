// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetUserData.h"
#include "Interfaces/MetaDataAssetInterface.h"
#include "Libraries/MetaData/FSocketTagMetaData.h"
#include "Objects/AssetUserData/MetaDataAssetUserData.h"
#include "MetaSocketAssetUserData.generated.h"

class USkeletalMesh;
class UStaticMesh;

struct FMetaDataTrait_Base;
/**
 * Helper child class that automatically detects the owning assets sockets, and adds and removes instanced structs for each one
 */
UCLASS()
class METADATAMESHSOCKET_API UMetaSocketAssetUserData :  public UAssetUserData, public IMetaDataExportInterface, public IMetaDataAssetInterface
{
	GENERATED_BODY()

protected:
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void SynchronizeSockets();

public:
	virtual void ExportTraits_Implementation(TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits,
		bool bClearArray) const override;
	virtual void RefreshMetaDataAsset_Implementation(UObject* OwningAsset) override;


protected:

	void ExtractSockets(const UStaticMesh* OwningAsset);
	void ExtractSockets(const USkeletalMesh* OwningAsset);

	void ProcessSockets(const TSet<FName>& ActiveSockets);
	
#endif

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FInternalSocketValue> InternalSocketData;
#endif
};
