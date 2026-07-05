// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MetaDataPlugin/Public/Libraries/FMetaDataRegistryItem.h"
#include "StructUtils/Public/InstancedStruct.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFMetaDataRegistryItem() {}

// Begin Cross Module References
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
METADATAPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FMetaDataRegistryItem();
STRUCTUTILS_API UScriptStruct* Z_Construct_UScriptStruct_FInstancedStruct();
UPackage* Z_Construct_UPackage__Script_MetaDataPlugin();
// End Cross Module References

// Begin ScriptStruct FMetaDataRegistryItem
static_assert(std::is_polymorphic<FMetaDataRegistryItem>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FMetaDataRegistryItem cannot be polymorphic unless super FTableRowBase is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_MetaDataRegistryItem;
class UScriptStruct* FMetaDataRegistryItem::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_MetaDataRegistryItem.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_MetaDataRegistryItem.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FMetaDataRegistryItem, (UObject*)Z_Construct_UPackage__Script_MetaDataPlugin(), TEXT("MetaDataRegistryItem"));
	}
	return Z_Registration_Info_UScriptStruct_MetaDataRegistryItem.OuterSingleton;
}
template<> METADATAPLUGIN_API UScriptStruct* StaticStruct<FMetaDataRegistryItem>()
{
	return FMetaDataRegistryItem::StaticStruct();
}
struct Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/Libraries/FMetaDataRegistryItem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ExtractedTraits_MetaData[] = {
		{ "Category", "MetaDataRegistryItem" },
		{ "ModuleRelativePath", "Public/Libraries/FMetaDataRegistryItem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_ExtractedTraits_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ExtractedTraits;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FMetaDataRegistryItem>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::NewProp_ExtractedTraits_Inner = { "ExtractedTraits", nullptr, (EPropertyFlags)0x0000000000020000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FInstancedStruct, METADATA_PARAMS(0, nullptr) }; // 174410355
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::NewProp_ExtractedTraits = { "ExtractedTraits", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FMetaDataRegistryItem, ExtractedTraits), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ExtractedTraits_MetaData), NewProp_ExtractedTraits_MetaData) }; // 174410355
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::NewProp_ExtractedTraits_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::NewProp_ExtractedTraits,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_MetaDataPlugin,
	Z_Construct_UScriptStruct_FTableRowBase,
	&NewStructOps,
	"MetaDataRegistryItem",
	Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::PropPointers),
	sizeof(FMetaDataRegistryItem),
	alignof(FMetaDataRegistryItem),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FMetaDataRegistryItem()
{
	if (!Z_Registration_Info_UScriptStruct_MetaDataRegistryItem.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_MetaDataRegistryItem.InnerSingleton, Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_MetaDataRegistryItem.InnerSingleton;
}
// End ScriptStruct FMetaDataRegistryItem

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Libraries_FMetaDataRegistryItem_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FMetaDataRegistryItem::StaticStruct, Z_Construct_UScriptStruct_FMetaDataRegistryItem_Statics::NewStructOps, TEXT("MetaDataRegistryItem"), &Z_Registration_Info_UScriptStruct_MetaDataRegistryItem, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FMetaDataRegistryItem), 3762029659U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Libraries_FMetaDataRegistryItem_h_1556411578(TEXT("/Script/MetaDataPlugin"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Libraries_FMetaDataRegistryItem_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Libraries_FMetaDataRegistryItem_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
