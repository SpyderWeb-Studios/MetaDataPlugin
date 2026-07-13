#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Libraries/Traits/FMetaDataTrait_Base.h"

#include "FGameplayTagTrait.generated.h"

USTRUCT(BlueprintType)
struct FGameplayTagTrait : public FMetaDataTrait_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer TraitTagContainer;
	
};