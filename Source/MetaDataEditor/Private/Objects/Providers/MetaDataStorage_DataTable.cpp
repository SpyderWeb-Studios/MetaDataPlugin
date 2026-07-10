// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Providers/MetaDataStorage_DataTable.h"

void UMetaDataStorage_DataTable::Flush_Implementation()
{
    if (TargetDataTable.IsNull())
    {
        return;
    }

    UDataTable* LoadedTable = TargetDataTable.LoadSynchronous();
    if (!IsValid(LoadedTable))
    {
        return;
    }

    // Call TargetTable->Modify() to alert the engine's undo/redo history.
    LoadedTable->Modify();
    for (const TPair<FName, FInstancedStruct>& CachedRow : CachedRows)
    {
        // 1. Get the raw memory pointer from the instanced struct
        const uint8* RawRowMemory = CachedRow.Value.GetMemory();

        if (RawRowMemory != nullptr)
        {
            // 2. Reinterpret cast the memory address to the base reference.
            // This avoids template type-matching assertions and prevents C++ object slicing.
            const FTableRowBase& RowDataRef = *reinterpret_cast<const FTableRowBase*>(RawRowMemory);

            // 3. Pass the polymorphic reference into the data table
            LoadedTable->AddRow(CachedRow.Key, RowDataRef);
        }
    }

    // Call TargetTable->MarkPackageDirty() so the editor prompts the user to save the modified table[cite: 160].
    LoadedTable->MarkPackageDirty();

    // Clear the cache so this provider instance can be safely reused
    CachedRows.Empty();
}

bool UMetaDataStorage_DataTable::ValidateTarget_Implementation(const UScriptStruct* TraitType) const
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

    // Compare TargetTable->RowStruct against your TraitType. If they do not match, fail gracefully[cite: 156, 157].
    if (LoadedTable->RowStruct != TraitType)
    {
        return false;
    }

    return true;
}

bool UMetaDataStorage_DataTable::ProcessMetadata_Implementation(const FName& RegistryKey, const FInstancedStruct& Payload)
{
    CachedRows.Add(RegistryKey, Payload);
    return true; // Added missing return statement
}