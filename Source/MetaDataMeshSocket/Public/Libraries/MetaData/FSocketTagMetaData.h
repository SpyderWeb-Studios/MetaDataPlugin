#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Libraries/MetaDataTraits/FGameplayTagTrait.h"

#include "FSocketTagMetaData.generated.h"

USTRUCT(BlueprintType)
struct FSocketTagMetaData : public FMetaDataTrait_Base
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName SocketName;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer SocketTagContainer;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagQuery SocketAttachmentQuery;
};

USTRUCT(BlueprintType)
struct FInternalSocketValue
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer SocketTagContainer;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagQuery SocketAttachmentQuery;
	
};
