// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MetaDataEditor/Public/Commandlets/MetaDataBakerCommandlet.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMetaDataBakerCommandlet() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UCommandlet();
METADATAEDITOR_API UClass* Z_Construct_UClass_UMetaDataBakerCommandlet();
METADATAEDITOR_API UClass* Z_Construct_UClass_UMetaDataBakerCommandlet_NoRegister();
UPackage* Z_Construct_UPackage__Script_MetaDataEditor();
// End Cross Module References

// Begin Class UMetaDataBakerCommandlet
void UMetaDataBakerCommandlet::StaticRegisterNativesUMetaDataBakerCommandlet()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMetaDataBakerCommandlet);
UClass* Z_Construct_UClass_UMetaDataBakerCommandlet_NoRegister()
{
	return UMetaDataBakerCommandlet::StaticClass();
}
struct Z_Construct_UClass_UMetaDataBakerCommandlet_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "Commandlets/MetaDataBakerCommandlet.h" },
		{ "ModuleRelativePath", "Public/Commandlets/MetaDataBakerCommandlet.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMetaDataBakerCommandlet>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UMetaDataBakerCommandlet_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UCommandlet,
	(UObject* (*)())Z_Construct_UPackage__Script_MetaDataEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataBakerCommandlet_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMetaDataBakerCommandlet_Statics::ClassParams = {
	&UMetaDataBakerCommandlet::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataBakerCommandlet_Statics::Class_MetaDataParams), Z_Construct_UClass_UMetaDataBakerCommandlet_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMetaDataBakerCommandlet()
{
	if (!Z_Registration_Info_UClass_UMetaDataBakerCommandlet.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMetaDataBakerCommandlet.OuterSingleton, Z_Construct_UClass_UMetaDataBakerCommandlet_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMetaDataBakerCommandlet.OuterSingleton;
}
template<> METADATAEDITOR_API UClass* StaticClass<UMetaDataBakerCommandlet>()
{
	return UMetaDataBakerCommandlet::StaticClass();
}
UMetaDataBakerCommandlet::UMetaDataBakerCommandlet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMetaDataBakerCommandlet);
UMetaDataBakerCommandlet::~UMetaDataBakerCommandlet() {}
// End Class UMetaDataBakerCommandlet

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_Commandlets_MetaDataBakerCommandlet_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMetaDataBakerCommandlet, UMetaDataBakerCommandlet::StaticClass, TEXT("UMetaDataBakerCommandlet"), &Z_Registration_Info_UClass_UMetaDataBakerCommandlet, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMetaDataBakerCommandlet), 1726127354U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_Commandlets_MetaDataBakerCommandlet_h_811038589(TEXT("/Script/MetaDataEditor"),
	Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_Commandlets_MetaDataBakerCommandlet_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_Commandlets_MetaDataBakerCommandlet_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
