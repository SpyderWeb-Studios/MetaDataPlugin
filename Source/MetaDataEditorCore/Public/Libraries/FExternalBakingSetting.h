#pragma once
#include "CoreMinimal.h"
#include "FExternalBakingSetting.generated.h"

USTRUCT(BlueprintType)
struct FExternalBakingSetting
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	bool bAutoScan = true;
	
};
