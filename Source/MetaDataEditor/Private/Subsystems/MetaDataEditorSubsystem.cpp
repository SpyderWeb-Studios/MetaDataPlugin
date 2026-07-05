// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MetaDataEditorSubsystem.h"
#include "InstancedStruct.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/ObjectSaveContext.h"

#include "Interfaces/MetaDataExportInterface.h"
#include "Libraries/FMetaDataRegistryItem.h"


void UMetaDataEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Bind to the asset deletion event
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    AssetRegistryModule.Get().OnAssetRemoved().AddUObject(this, &UMetaDataEditorSubsystem::OnAssetDeleted);

    // 
    UPackage::PreSavePackageWithContextEvent.AddUObject(this, &UMetaDataEditorSubsystem::OnPreSavePackage);
}

void UMetaDataEditorSubsystem::Deinitialize()
{
    if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
    {
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        AssetRegistryModule.Get().OnAssetRemoved().RemoveAll(this);
    }

    UPackage::PreSavePackageWithContextEvent.RemoveAll(this);
    
    Super::Deinitialize();
}

void UMetaDataEditorSubsystem::OnAssetDeleted(const FAssetData& AssetData)
{
    // The RowName is the path of the deleted asset
    FName RowNameToRemove = FName(*AssetData.GetObjectPathString());

    // Check all metadata tables and strip out the deleted asset
    TArray<UDataTable*> AllMetaTables = GetAllMetadataTables();
    for (UDataTable* Table : AllMetaTables)
    {
        if (Table && Table->GetRowMap().Contains(RowNameToRemove))
        {
            Table->RemoveRow(RowNameToRemove);
            Table->MarkPackageDirty();
            UE_LOG(LogTemp, Display, TEXT("Asset Deleted: Cleaned up row [%s] from [%s]"), *RowNameToRemove.ToString(), *Table->GetName());
        }
    }
}


TArray<UDataTable*> UMetaDataEditorSubsystem::GetAllMetadataTables()
{
    TArray<UDataTable*> FoundTables;
    
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> AssetDataList;
    
    // Search for all Data Tables
    AssetRegistryModule.Get().GetAssetsByClass(UDataTable::StaticClass()->GetClassPathName(), AssetDataList);

    for (const FAssetData& AssetData : AssetDataList)
    {
        // Only load the table if it uses our specific Registry Item Struct
        FString RowStructName;
        if (AssetData.GetTagValue(FName("RowStructure"), RowStructName))
        {
            if (RowStructName.Contains(TEXT("MetaDataRegistryItem")))
            {
                if (UDataTable* Table = Cast<UDataTable>(AssetData.GetAsset()))
                {
                    FoundTables.Add(Table);
                }
            }
        }
    }
    return FoundTables;
}

void UMetaDataEditorSubsystem::OnPreSavePackage(UPackage* Package, FObjectPreSaveContext Context)
{
    // Ignore autosaves or cooking saves to prevent infinite loops
    if (Context.IsProceduralSave() || Context.IsCooking()) return;

    if (!Package) return;

    TArray<UObject*> ObjectsInPackage;
    GetObjectsWithOuter(Package, ObjectsInPackage, false);

    for (UObject* Obj : ObjectsInPackage)
    {
        // 1. Check if the object is either a Static Mesh or a Skeletal Mesh
        if (Obj->IsA<UStaticMesh>() || Obj->IsA<USkeletalMesh>())
        {
            ExtractAndBakeMetadata(Obj);
        }
    }
}

void UMetaDataEditorSubsystem::ExtractAndBakeMetadata(UObject* MeshAsset)
{
    if (!MeshAsset) return;

    // 1. Safely retrieve the AssetUserData array based on the mesh type
    const TArray<UAssetUserData*>* UserDataArray = nullptr;

    if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(MeshAsset))
    {
        UserDataArray = StaticMesh->GetAssetUserDataArray();
    }
    else if (USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(MeshAsset))
    {
        UserDataArray = SkeletalMesh->GetAssetUserDataArray();
    }

    // If it's neither, or doesn't have an array, bail out early
    if (!UserDataArray) return;

    UE_LOG(LogTemp, Display, TEXT("Extracting data from [%s]"), *GetNameSafe(MeshAsset));
    TArray<TInstancedStruct<FMetaDataTrait_Base>> SourceTraits;
    
    // Extract meta data from user asset data
    for (UAssetUserData* UserData : *UserDataArray)
    {
        if (UserData && UserData->Implements<UMetaDataExportInterface>())
        {
            IMetaDataExportInterface::Execute_ExportTraits(UserData, SourceTraits, false);
        }
    }

    UE_LOG(LogTemp, Display, TEXT("Found [%d] Traits to add to data registry"), SourceTraits.Num());

    // 1. Group the extracted traits by their target TraitRegistry
    TMap<UDataTable*, TArray<TInstancedStruct<FMetaDataTrait_Base>>> TraitsByRegistry;

    for (const TInstancedStruct<FMetaDataTrait_Base>& Trait : SourceTraits)
    {
        if (Trait.IsValid())
        {
            TSoftObjectPtr<UDataTable> TargetRegistry = Trait.Get<FMetaDataTrait_Base>().TraitRegistry;
            if (UDataTable* LoadedTable = TargetRegistry.LoadSynchronous())
            {
                TraitsByRegistry.FindOrAdd(LoadedTable).Add(Trait);
            }
        }
    }

    const FName RowName = FName(*MeshAsset->GetPathName());

    // 2. Dynamically find ALL metadata tables in the project
    TArray<UDataTable*> AllMetaTables = GetAllMetadataTables();

    // 3. Iterate through EVERY table to either update it or clean it up
    for (UDataTable* RegistryTable : AllMetaTables)
    {
        // Does this table have new traits incoming for this mesh?
        if (TArray<TInstancedStruct<FMetaDataTrait_Base>>* IncomingTraits = TraitsByRegistry.Find(RegistryTable))
        {
            // Update the row (Trait exists or was added)
            FMetaDataRegistryItem NewRow;
            if (FMetaDataRegistryItem* ExistingRow = RegistryTable->FindRow<FMetaDataRegistryItem>(RowName, TEXT("Updating Row")))
            {
                NewRow = *ExistingRow;
            }
            
            NewRow.ExtractedTraits.Empty(); 
            NewRow.ExtractedTraits.Append(*IncomingTraits);
            
            RegistryTable->AddRow(RowName, NewRow);
            RegistryTable->MarkPackageDirty();
            ModifiedTables.Add(RegistryTable);
        }
        else
        {
            // NO incoming traits for this table. 
            // If the row exists, it means the trait was removed by the user!
            if (RegistryTable->GetRowMap().Contains(RowName))
            {
                RegistryTable->RemoveRow(RowName);
                RegistryTable->MarkPackageDirty();
                ModifiedTables.Add(RegistryTable);
                UE_LOG(LogTemp, Warning, TEXT("Removed stale row [%s] from [%s] because the trait was removed."), *RowName.ToString(), *RegistryTable->GetName());
            }
        }
    }

}