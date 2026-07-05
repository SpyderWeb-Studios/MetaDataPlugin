// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMetaDataPlugin_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_MetaDataPlugin;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_MetaDataPlugin()
	{
		if (!Z_Registration_Info_UPackage__Script_MetaDataPlugin.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/MetaDataPlugin",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0xFA40EF16,
				0xD0A673FD,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_MetaDataPlugin.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_MetaDataPlugin.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_MetaDataPlugin(Z_Construct_UPackage__Script_MetaDataPlugin, TEXT("/Script/MetaDataPlugin"), Z_Registration_Info_UPackage__Script_MetaDataPlugin, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xFA40EF16, 0xD0A673FD));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
