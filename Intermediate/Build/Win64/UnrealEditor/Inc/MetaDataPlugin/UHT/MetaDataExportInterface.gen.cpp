// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MetaDataPlugin/Public/Interfaces/MetaDataExportInterface.h"
#include "StructUtils/Public/InstancedStruct.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMetaDataExportInterface() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
METADATAPLUGIN_API UClass* Z_Construct_UClass_UMetaDataExportInterface();
METADATAPLUGIN_API UClass* Z_Construct_UClass_UMetaDataExportInterface_NoRegister();
STRUCTUTILS_API UScriptStruct* Z_Construct_UScriptStruct_FInstancedStruct();
UPackage* Z_Construct_UPackage__Script_MetaDataPlugin();
// End Cross Module References

// Begin Interface UMetaDataExportInterface Function ExportTraits
struct MetaDataExportInterface_eventExportTraits_Parms
{
	TArray<TInstancedStruct<FMetaDataTrait_Base> > OutTraits;
	bool bClearArray;
};
void IMetaDataExportInterface::ExportTraits(TArray<TInstancedStruct<FMetaDataTrait_Base> >& OutTraits, bool bClearArray) const
{
	check(0 && "Do not directly call Event functions in Interfaces. Call Execute_ExportTraits instead.");
}
static FName NAME_UMetaDataExportInterface_ExportTraits = FName(TEXT("ExportTraits"));
void IMetaDataExportInterface::Execute_ExportTraits(const UObject* O, TArray<TInstancedStruct<FMetaDataTrait_Base> >& OutTraits, bool bClearArray)
{
	check(O != NULL);
	check(O->GetClass()->ImplementsInterface(UMetaDataExportInterface::StaticClass()));
	MetaDataExportInterface_eventExportTraits_Parms Parms;
	UFunction* const Func = O->FindFunction(NAME_UMetaDataExportInterface_ExportTraits);
	if (Func)
	{
		Parms.OutTraits=OutTraits;
		Parms.bClearArray=bClearArray;
		const_cast<UObject*>(O)->ProcessEvent(Func, &Parms);
		OutTraits=Parms.OutTraits;
	}
	else if (auto I = (const IMetaDataExportInterface*)(O->GetNativeInterfaceAddress(UMetaDataExportInterface::StaticClass())))
	{
		I->ExportTraits_Implementation(OutTraits,bClearArray);
	}
}
struct Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Automation" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// The automation tool calls this to get the inline objects\n" },
#endif
		{ "ModuleRelativePath", "Public/Interfaces/MetaDataExportInterface.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The automation tool calls this to get the inline objects" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_OutTraits_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_OutTraits;
	static void NewProp_bClearArray_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bClearArray;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::NewProp_OutTraits_Inner = { "OutTraits", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FInstancedStruct, METADATA_PARAMS(0, nullptr) }; // 174410355
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::NewProp_OutTraits = { "OutTraits", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(MetaDataExportInterface_eventExportTraits_Parms, OutTraits), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) }; // 174410355
void Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::NewProp_bClearArray_SetBit(void* Obj)
{
	((MetaDataExportInterface_eventExportTraits_Parms*)Obj)->bClearArray = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::NewProp_bClearArray = { "bClearArray", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(MetaDataExportInterface_eventExportTraits_Parms), &Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::NewProp_bClearArray_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::NewProp_OutTraits_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::NewProp_OutTraits,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::NewProp_bClearArray,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMetaDataExportInterface, nullptr, "ExportTraits", nullptr, nullptr, Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::PropPointers), sizeof(MetaDataExportInterface_eventExportTraits_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x5C420C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::Function_MetaDataParams) };
static_assert(sizeof(MetaDataExportInterface_eventExportTraits_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(IMetaDataExportInterface::execExportTraits)
{
	P_GET_TARRAY_REF(TInstancedStruct<FMetaDataTrait_Base>,Z_Param_Out_OutTraits);
	P_GET_UBOOL(Z_Param_bClearArray);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ExportTraits_Implementation(Z_Param_Out_OutTraits,Z_Param_bClearArray);
	P_NATIVE_END;
}
// End Interface UMetaDataExportInterface Function ExportTraits

// Begin Interface UMetaDataExportInterface
void UMetaDataExportInterface::StaticRegisterNativesUMetaDataExportInterface()
{
	UClass* Class = UMetaDataExportInterface::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ExportTraits", &IMetaDataExportInterface::execExportTraits },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMetaDataExportInterface);
UClass* Z_Construct_UClass_UMetaDataExportInterface_NoRegister()
{
	return UMetaDataExportInterface::StaticClass();
}
struct Z_Construct_UClass_UMetaDataExportInterface_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/Interfaces/MetaDataExportInterface.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UMetaDataExportInterface_ExportTraits, "ExportTraits" }, // 2540264188
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IMetaDataExportInterface>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UMetaDataExportInterface_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UInterface,
	(UObject* (*)())Z_Construct_UPackage__Script_MetaDataPlugin,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataExportInterface_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMetaDataExportInterface_Statics::ClassParams = {
	&UMetaDataExportInterface::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x000840A1u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMetaDataExportInterface_Statics::Class_MetaDataParams), Z_Construct_UClass_UMetaDataExportInterface_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMetaDataExportInterface()
{
	if (!Z_Registration_Info_UClass_UMetaDataExportInterface.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMetaDataExportInterface.OuterSingleton, Z_Construct_UClass_UMetaDataExportInterface_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMetaDataExportInterface.OuterSingleton;
}
template<> METADATAPLUGIN_API UClass* StaticClass<UMetaDataExportInterface>()
{
	return UMetaDataExportInterface::StaticClass();
}
UMetaDataExportInterface::UMetaDataExportInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMetaDataExportInterface);
UMetaDataExportInterface::~UMetaDataExportInterface() {}
// End Interface UMetaDataExportInterface

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMetaDataExportInterface, UMetaDataExportInterface::StaticClass, TEXT("UMetaDataExportInterface"), &Z_Registration_Info_UClass_UMetaDataExportInterface, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMetaDataExportInterface), 1892146486U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_1641422639(TEXT("/Script/MetaDataPlugin"),
	Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
