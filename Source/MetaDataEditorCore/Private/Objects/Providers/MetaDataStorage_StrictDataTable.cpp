// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Providers/MetaDataStorage_StrictDataTable.h"

bool UMetaDataStorage_StrictDataTable::ValidateTarget_Implementation(const UScriptStruct* TraitType) const
{
	if (TargetDataTable.IsNull())
	{
		return false;
	}

	UDataTable* LoadedTable = TargetDataTable.LoadSynchronous();
	if (!IsValid(LoadedTable))
	{
		return false;
	}

	// CRITICAL: Ensure the Trait's C++ struct exactly matches the Data Table's Row Struct
	if (LoadedTable->RowStruct != TraitType)
	{
		UE_LOG(LogTemp, Error, TEXT("Bake Failed: Trait schema [%s] does not match Data Table schema [%s] in %s."), 
			*TraitType->GetName(), 
			*LoadedTable->RowStruct->GetName(), 
			*LoadedTable->GetName());
		return false;
	}

	return true;
}

void UMetaDataStorage_StrictDataTable::Flush_Implementation()
{
	if (TargetDataTable.IsNull()) return;
    
	UDataTable* LoadedTable = TargetDataTable.LoadSynchronous();
	if (!IsValid(LoadedTable)) return;

	LoadedTable->Modify();

	for (const TPair<FName, TInstancedStruct<FMetaDataTrait_Base>>& CachedTrait : CachedTraits)
	{
		// 1. Get the raw memory pointer from the Instanced Struct
		const uint8* TraitMemory = CachedTrait.Value.GetMemory();

		// 2. Safely cast the memory to FTableRowBase since we validated the RowStruct earlier
		const FTableRowBase* RowData = reinterpret_cast<const FTableRowBase*>(TraitMemory);

		// 3. Inject the columns directly into the table
		LoadedTable->AddRow(CachedTrait.Key, *RowData);
	}

	LoadedTable->MarkPackageDirty();
	Execute_ClearCache(this);
}

void UMetaDataStorage_StrictDataTable::ClearCache_Implementation()
{
	CachedTraits.Empty();
}

bool UMetaDataStorage_StrictDataTable::ProcessMetadata_Implementation(const FDataRegistryId& RegistryKey,
	const TInstancedStruct<FMetaDataTrait_Base>& Payload, const TSoftObjectPtr<>& UnderlyingAsset)
{
	CachedTraits.Add(RegistryKey.ItemName, Payload);
	return true;
}

FDataRegistryType UMetaDataStorage_StrictDataTable::GetDataRegistryType_Implementation() const
{
	return DataRegistryType;
}
