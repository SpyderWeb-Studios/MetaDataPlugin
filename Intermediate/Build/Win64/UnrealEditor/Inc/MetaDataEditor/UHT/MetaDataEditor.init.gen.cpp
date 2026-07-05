// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMetaDataEditor_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_MetaDataEditor;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_MetaDataEditor()
	{
		if (!Z_Registration_Info_UPackage__Script_MetaDataEditor.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/MetaDataEditor",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000040,
				0xD0366C26,
				0x1C82A705,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_MetaDataEditor.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_MetaDataEditor.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_MetaDataEditor(Z_Construct_UPackage__Script_MetaDataEditor, TEXT("/Script/MetaDataEditor"), Z_Registration_Info_UPackage__Script_MetaDataEditor, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xD0366C26, 0x1C82A705));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
