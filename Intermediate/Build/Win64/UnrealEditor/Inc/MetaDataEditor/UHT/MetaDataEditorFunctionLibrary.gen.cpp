// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MetaDataEditor/Public/FunctionLibraries/MetaDataEditorFunctionLibrary.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMetaDataEditorFunctionLibrary() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
METADATAEDITOR_API UClass* Z_Construct_UClass_UMetaDataEditorFunctionLibrary();
METADATAEDITOR_API UClass* Z_Construct_UClass_UMetaDataEditorFunctionLibrary_NoRegister();
UPackage* Z_Construct_UPackage__Script_MetaDataEditor();
// End Cross Module References

// Begin Class UMetaDataEditorFunctionLibrary
void UMetaDataEditorFunctionLibrary::StaticRegisterNativesUMetaDataEditorFunctionLibrary()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMetaDataEditorFunctionLibrary);
UClass* Z_Construct_UClass_UMetaDataEditorFunctionLibrary_NoRegister()
{
	return UMetaDataEditorFunctionLibrary::StaticClass();
}
struct Z_Construct_UClass_UMetaDataEditorFunctionLibrary_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "FunctionLibraries/MetaDataEditorFunctionLibrary.h" },
		{ "ModuleRelativePath", "Public/FunctionLibraries/MetaDataEditorFunctionLibrary.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMetaDataEditorFunctionLibrary>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UMetaDataEditorFunctionLibrary_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_MetaDataEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataEditorFunctionLibrary_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMetaDataEditorFunctionLibrary_Statics::ClassParams = {
	&UMetaDataEditorFunctionLibrary::StaticClass,
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
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataEditorFunctionLibrary_Statics::Class_MetaDataParams), Z_Construct_UClass_UMetaDataEditorFunctionLibrary_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMetaDataEditorFunctionLibrary()
{
	if (!Z_Registration_Info_UClass_UMetaDataEditorFunctionLibrary.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMetaDataEditorFunctionLibrary.OuterSingleton, Z_Construct_UClass_UMetaDataEditorFunctionLibrary_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMetaDataEditorFunctionLibrary.OuterSingleton;
}
template<> METADATAEDITOR_API UClass* StaticClass<UMetaDataEditorFunctionLibrary>()
{
	return UMetaDataEditorFunctionLibrary::StaticClass();
}
UMetaDataEditorFunctionLibrary::UMetaDataEditorFunctionLibrary(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMetaDataEditorFunctionLibrary);
UMetaDataEditorFunctionLibrary::~UMetaDataEditorFunctionLibrary() {}
// End Class UMetaDataEditorFunctionLibrary

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_FunctionLibraries_MetaDataEditorFunctionLibrary_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMetaDataEditorFunctionLibrary, UMetaDataEditorFunctionLibrary::StaticClass, TEXT("UMetaDataEditorFunctionLibrary"), &Z_Registration_Info_UClass_UMetaDataEditorFunctionLibrary, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMetaDataEditorFunctionLibrary), 797115295U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_FunctionLibraries_MetaDataEditorFunctionLibrary_h_665791911(TEXT("/Script/MetaDataEditor"),
	Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_FunctionLibraries_MetaDataEditorFunctionLibrary_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_FunctionLibraries_MetaDataEditorFunctionLibrary_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
