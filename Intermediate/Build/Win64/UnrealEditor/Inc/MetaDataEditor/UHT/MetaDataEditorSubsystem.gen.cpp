// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MetaDataEditor/Public/Subsystems/MetaDataEditorSubsystem.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMetaDataEditorSubsystem() {}

// Begin Cross Module References
EDITORSUBSYSTEM_API UClass* Z_Construct_UClass_UEditorSubsystem();
ENGINE_API UClass* Z_Construct_UClass_UDataTable_NoRegister();
METADATAEDITOR_API UClass* Z_Construct_UClass_UMetaDataEditorSubsystem();
METADATAEDITOR_API UClass* Z_Construct_UClass_UMetaDataEditorSubsystem_NoRegister();
UPackage* Z_Construct_UPackage__Script_MetaDataEditor();
// End Cross Module References

// Begin Class UMetaDataEditorSubsystem
void UMetaDataEditorSubsystem::StaticRegisterNativesUMetaDataEditorSubsystem()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMetaDataEditorSubsystem);
UClass* Z_Construct_UClass_UMetaDataEditorSubsystem_NoRegister()
{
	return UMetaDataEditorSubsystem::StaticClass();
}
struct Z_Construct_UClass_UMetaDataEditorSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "Subsystems/MetaDataEditorSubsystem.h" },
		{ "ModuleRelativePath", "Public/Subsystems/MetaDataEditorSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ModifiedTables_MetaData[] = {
		{ "Category", "MetaDataEditorSubsystem" },
		{ "ModuleRelativePath", "Public/Subsystems/MetaDataEditorSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ModifiedTables_ElementProp;
	static const UECodeGen_Private::FSetPropertyParams NewProp_ModifiedTables;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMetaDataEditorSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::NewProp_ModifiedTables_ElementProp = { "ModifiedTables", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FSetPropertyParams Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::NewProp_ModifiedTables = { "ModifiedTables", nullptr, (EPropertyFlags)0x0020080000000014, UECodeGen_Private::EPropertyGenFlags::Set, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UMetaDataEditorSubsystem, ModifiedTables), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ModifiedTables_MetaData), NewProp_ModifiedTables_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::NewProp_ModifiedTables_ElementProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::NewProp_ModifiedTables,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEditorSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_MetaDataEditor,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::ClassParams = {
	&UMetaDataEditorSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMetaDataEditorSubsystem()
{
	if (!Z_Registration_Info_UClass_UMetaDataEditorSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMetaDataEditorSubsystem.OuterSingleton, Z_Construct_UClass_UMetaDataEditorSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMetaDataEditorSubsystem.OuterSingleton;
}
template<> METADATAEDITOR_API UClass* StaticClass<UMetaDataEditorSubsystem>()
{
	return UMetaDataEditorSubsystem::StaticClass();
}
UMetaDataEditorSubsystem::UMetaDataEditorSubsystem() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMetaDataEditorSubsystem);
UMetaDataEditorSubsystem::~UMetaDataEditorSubsystem() {}
// End Class UMetaDataEditorSubsystem

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_Subsystems_MetaDataEditorSubsystem_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMetaDataEditorSubsystem, UMetaDataEditorSubsystem::StaticClass, TEXT("UMetaDataEditorSubsystem"), &Z_Registration_Info_UClass_UMetaDataEditorSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMetaDataEditorSubsystem), 3140831251U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_Subsystems_MetaDataEditorSubsystem_h_2672881603(TEXT("/Script/MetaDataEditor"),
	Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_Subsystems_MetaDataEditorSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataEditor_Public_Subsystems_MetaDataEditorSubsystem_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
