// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MetaDataPlugin/Public/Libraries/Traits/FMetaDataTrait_Base.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFMetaDataTrait_Base() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UDataTable_NoRegister();
METADATAPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMetaDataTrait_Base();
UPackage* Z_Construct_UPackage__Script_MetaDataPlugin();
// End Cross Module References

// Begin ScriptStruct FMetaDataTrait_Base
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_MetaDataTrait_Base;
class UScriptStruct* FMetaDataTrait_Base::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_MetaDataTrait_Base.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_MetaDataTrait_Base.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMetaDataTrait_Base, (UObject*)Z_Construct_UPackage__Script_MetaDataPlugin(), TEXT("MetaDataTrait_Base"));
	}
	return Z_Registration_Info_UScriptStruct_MetaDataTrait_Base.OuterSingleton;
}
template<> METADATAPLUGIN_API UScriptStruct* StaticStruct<FMetaDataTrait_Base>()
{
	return FMetaDataTrait_Base::StaticStruct();
}
struct Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * Struct that holds the basic meta data about an asset\n */" },
#endif
		{ "ModuleRelativePath", "Public/Libraries/Traits/FMetaDataTrait_Base.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Struct that holds the basic meta data about an asset" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_TraitRegistry_MetaData[] = {
		{ "Category", "MetaDataTrait_Base" },
		{ "ModuleRelativePath", "Public/Libraries/Traits/FMetaDataTrait_Base.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FSoftObjectPropertyParams NewProp_TraitRegistry;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMetaDataTrait_Base>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FSoftObjectPropertyParams Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::NewProp_TraitRegistry = { "TraitRegistry", nullptr, (EPropertyFlags)0x0014040000010001, UECodeGen_Private::EPropertyGenFlags::SoftObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMetaDataTrait_Base, TraitRegistry), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_TraitRegistry_MetaData), NewProp_TraitRegistry_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::NewProp_TraitRegistry,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_MetaDataPlugin,
	nullptr,
	&NewStructOps,
	"MetaDataTrait_Base",
	Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::PropPointers),
	sizeof(FMetaDataTrait_Base),
	alignof(FMetaDataTrait_Base),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMetaDataTrait_Base()
{
	if (!Z_Registration_Info_UScriptStruct_MetaDataTrait_Base.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_MetaDataTrait_Base.InnerSingleton, Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_MetaDataTrait_Base.InnerSingleton;
}
// End ScriptStruct FMetaDataTrait_Base

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Libraries_Traits_FMetaDataTrait_Base_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMetaDataTrait_Base::StaticStruct, Z_Construct_UScriptStruct_FMetaDataTrait_Base_Statics::NewStructOps, TEXT("MetaDataTrait_Base"), &Z_Registration_Info_UScriptStruct_MetaDataTrait_Base, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMetaDataTrait_Base), 3740991088U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Libraries_Traits_FMetaDataTrait_Base_h_1694014590(TEXT("/Script/MetaDataPlugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Libraries_Traits_FMetaDataTrait_Base_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Libraries_Traits_FMetaDataTrait_Base_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
