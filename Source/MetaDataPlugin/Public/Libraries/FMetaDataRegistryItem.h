#pragma once
#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "StructUtils/InstancedStruct.h"
#include "Engine/DataTable.h"
#include "Traits/FMetaDataTrait_Base.h"
#include "FMetaDataRegistryItem.generated.h"

USTRUCT(BlueprintType)
struct FMetaDataRegistryItem : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TInstancedStruct<FMetaDataTrait_Base>> ExtractedTraits;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FSoftObjectPath AssetPath;
};
