// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Providers/MetaDataStorage_DataTable.h"

void UMetaDataStorage_DataTable::Flush_Implementation()
{
    if (TargetDataTable.IsNull())
    {
        UE_LOG(LogMetaDataStorageProvider, Error, TEXT("Flush failed: Target Data Table [%s] is null when load attempted ."), *TargetDataTable.ToString());
        return;
    }

    UDataTable* LoadedTable = TargetDataTable.LoadSynchronous();
    if (!IsValid(LoadedTable))
    {
        UE_LOG(LogMetaDataStorageProvider, Error, TEXT("Flush failed: Target Data Table [%s] not valid when load attempted ."), *GetNameSafe(LoadedTable));

        return;
    }
    
    // Safety Check: Ensure external users haven't assigned an incompatible Data Table
    if (LoadedTable->RowStruct != FMetaDataRegistryItem::StaticStruct())
    {
        UE_LOG(LogMetaDataStorageProvider, Error, TEXT("Flush failed: Target Data Table '%s' is not configured to use the FMetaDataRegistryItem row struct."), *LoadedTable->GetName());
        return;
    }
    
    // Call TargetTable->Modify() to alert the engine's undo/redo history.
    LoadedTable->Modify();
    for (TPair<FDataRegistryId, FMetaDataRegistryItem> CachedRow : CachedRows)
    {
        // 3. Pass the polymorphic reference into the data table
        LoadedTable->AddRow(CachedRow.Key.ItemName, CachedRow.Value);
    
    }

    // Call TargetTable->MarkPackageDirty() so the editor prompts the user to save the modified table[cite: 160].
    LoadedTable->MarkPackageDirty();
}

void UMetaDataStorage_DataTable::ClearCache_Implementation()
{
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

bool UMetaDataStorage_DataTable::ProcessMetadata_Implementation(const FDataRegistryId& RegistryKey, const TInstancedStruct<FMetaDataTrait_Base>& Payload, const TSoftObjectPtr<UObject>& UnderlyingAsset)
{
    FMetaDataRegistryItem& Row = CachedRows.FindOrAdd(RegistryKey);
    Row.ExtractedTraits.Add(Payload);
    Row.AssetPath = UnderlyingAsset.ToSoftObjectPath();
    
    return true; // Added missing return statement
}