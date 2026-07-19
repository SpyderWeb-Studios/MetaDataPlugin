// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetUserData/MetaSocketAssetUserData.h"

#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshSocket.h"

#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"

INCLUDE_INSTANCED_STRUCTS
#include "Libraries/MetaData/FSocketTagMetaData.h"

#if WITH_EDITOR
void UMetaSocketAssetUserData::PostLoad()
{
	Super::PostLoad();
	SynchronizeSockets();
}

void UMetaSocketAssetUserData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SynchronizeSockets();
}

void UMetaSocketAssetUserData::SynchronizeSockets()
{
	if(GetOuter()->IsA(UStaticMesh::StaticClass()))
	{
		const UStaticMesh* OwningMesh = Cast<UStaticMesh>(GetOuter());
		ExtractSockets(OwningMesh);
	}
	else if(GetOuter()->IsA(USkeletalMesh::StaticClass()))
	{
		const USkeletalMesh* OwningMesh = Cast<USkeletalMesh>(GetOuter());
		ExtractSockets(OwningMesh);
	}
}

void UMetaSocketAssetUserData::ExportTraits_Implementation(TArray<TInstancedStruct<FMetaDataTrait_Base>>& OutTraits,
	bool bClearArray) const
{
	for(const TTuple<FName, FInternalSocketValue> & TagData : InternalSocketData)
	{
		TInstancedStruct<FSocketTagMetaData> ExportedSocketData;
		ExportedSocketData.GetMutablePtr<FSocketTagMetaData>()->SocketName = TagData.Key;
		ExportedSocketData.GetMutablePtr<FSocketTagMetaData>()->SocketAttachmentQuery = TagData.Value.SocketAttachmentQuery;
		ExportedSocketData.GetMutablePtr<FSocketTagMetaData>()->SocketTagContainer = TagData.Value.SocketTagContainer;
		
		OutTraits.Add(ExportedSocketData);
	}
}

void UMetaSocketAssetUserData::RefreshMetaDataAsset_Implementation(UObject* OwningAsset)
{
	SynchronizeSockets();
}

void UMetaSocketAssetUserData::ExtractSockets(const UStaticMesh* OwningAsset)
{
	if (!OwningAsset) return;
	
	TSet<FName> ActualSocketNames;
	for(const UStaticMeshSocket* Socket : OwningAsset->Sockets)
	{
		if(!Socket)
		{
			continue;
		}
		
		ActualSocketNames.Add(Socket->SocketName);
	}

	ProcessSockets(ActualSocketNames);
}

void UMetaSocketAssetUserData::ExtractSockets(const USkeletalMesh* OwningAsset)
{
	if (!OwningAsset) return;
	
	TSet<FName> ActualSocketNames;
	for(const USkeletalMeshSocket* Socket : OwningAsset->GetActiveSocketList())
	{
		if(!Socket)
		{
			continue;
		}
		
		ActualSocketNames.Add(Socket->SocketName);
	}

	ProcessSockets(ActualSocketNames);
}

void UMetaSocketAssetUserData::ProcessSockets(const TSet<FName>& ActiveSockets)
{
	
	TSet<FName> InactiveSockets;
	InternalSocketData.GetKeys(InactiveSockets);
	
	bool bDataChanged = false;
		
	for (const FName& Socket : ActiveSockets)
	{
		InactiveSockets.Remove(Socket);

		if (!InternalSocketData.Contains(Socket))
		{
			InternalSocketData.Add(Socket, FInternalSocketValue());
			bDataChanged = true;
		}
	}

	// Remove sockets that no longer exist on the mesh
	for (const FName& SocketName : InactiveSockets)
	{
		InternalSocketData.Remove(SocketName);
		bDataChanged = true;
	}

	// Mark the object as modified so the Engine knows to save these changes
	if (bDataChanged)
	{
		Modify();
	}
}


#endif
