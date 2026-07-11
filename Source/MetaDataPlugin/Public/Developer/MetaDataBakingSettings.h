// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MetaDataBakingSettings.generated.h"

/**
 * 
 */
UCLASS(config=Game, DefaultConfig)
class METADATAPLUGIN_API UMetaDataBakingSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	//~ Begin UDeveloperSettings interface
	virtual FName GetCategoryName() const;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif
	//~ End UDeveloperSettings interface

#if WITH_EDITOR

	UFUNCTION(BlueprintCallable)
	bool ShouldRemoveMetaDataByDefault() const {return bGlobalRemoveMetaData;}
	UFUNCTION(BlueprintCallable)
	bool ShouldOverrideLocalEditorOnly() const {return bOverrideLocalEditorOnly;}

#endif


protected:

#if WITH_EDITORONLY_DATA

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	bool bGlobalRemoveMetaData = true;

	UPROPERTY(Config, EditDefaultsOnly)
	bool bOverrideLocalEditorOnly = false;
	
#endif
	

	
};
