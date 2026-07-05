// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MetaDataPlugin/Public/Subsystems/MetaDataRuntimeSubsystem.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMetaDataRuntimeSubsystem() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UGameInstanceSubsystem();
METADATAPLUGIN_API UClass* Z_Construct_UClass_UMetaDataRuntimeSubsystem();
METADATAPLUGIN_API UClass* Z_Construct_UClass_UMetaDataRuntimeSubsystem_NoRegister();
UPackage* Z_Construct_UPackage__Script_MetaDataPlugin();
// End Cross Module References

// Begin Class UMetaDataRuntimeSubsystem
void UMetaDataRuntimeSubsystem::StaticRegisterNativesUMetaDataRuntimeSubsystem()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMetaDataRuntimeSubsystem);
UClass* Z_Construct_UClass_UMetaDataRuntimeSubsystem_NoRegister()
{
	return UMetaDataRuntimeSubsystem::StaticClass();
}
struct Z_Construct_UClass_UMetaDataRuntimeSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "Subsystems/MetaDataRuntimeSubsystem.h" },
		{ "ModuleRelativePath", "Public/Subsystems/MetaDataRuntimeSubsystem.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMetaDataRuntimeSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UMetaDataRuntimeSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstanceSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_MetaDataPlugin,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataRuntimeSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMetaDataRuntimeSubsystem_Statics::ClassParams = {
	&UMetaDataRuntimeSubsystem::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataRuntimeSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UMetaDataRuntimeSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMetaDataRuntimeSubsystem()
{
	if (!Z_Registration_Info_UClass_UMetaDataRuntimeSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMetaDataRuntimeSubsystem.OuterSingleton, Z_Construct_UClass_UMetaDataRuntimeSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMetaDataRuntimeSubsystem.OuterSingleton;
}
template<> METADATAPLUGIN_API UClass* StaticClass<UMetaDataRuntimeSubsystem>()
{
	return UMetaDataRuntimeSubsystem::StaticClass();
}
UMetaDataRuntimeSubsystem::UMetaDataRuntimeSubsystem() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMetaDataRuntimeSubsystem);
UMetaDataRuntimeSubsystem::~UMetaDataRuntimeSubsystem() {}
// End Class UMetaDataRuntimeSubsystem

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Subsystems_MetaDataRuntimeSubsystem_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMetaDataRuntimeSubsystem, UMetaDataRuntimeSubsystem::StaticClass, TEXT("UMetaDataRuntimeSubsystem"), &Z_Registration_Info_UClass_UMetaDataRuntimeSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMetaDataRuntimeSubsystem), 2092586894U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Subsystems_MetaDataRuntimeSubsystem_h_2635405892(TEXT("/Script/MetaDataPlugin"),
	Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Subsystems_MetaDataRuntimeSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Subsystems_MetaDataRuntimeSubsystem_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
