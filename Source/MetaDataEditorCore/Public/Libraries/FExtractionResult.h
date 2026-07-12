#pragma once
#include "CoreMinimal.h"
#include "FExtractionResult.generated.h"


USTRUCT(BlueprintType)
struct FMetaDataExtractionResult
{

	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	bool bHasTraits = false;

	UPROPERTY(VisibleAnywhere)
	bool bTraitsSuccessfullyExtracted = false;
	
};