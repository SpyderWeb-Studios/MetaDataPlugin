#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Libraries/Traits/FMetaDataTrait_Base.h"

#include "FGameplayTagQueryTrait.generated.h"

USTRUCT(BlueprintType)
struct FGameplayTagQueryTrait : public FMetaDataTrait_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagQuery TraitTagQuery;
	
};