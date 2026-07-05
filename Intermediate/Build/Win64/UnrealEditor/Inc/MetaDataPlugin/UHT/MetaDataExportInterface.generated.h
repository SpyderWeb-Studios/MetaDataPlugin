// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Interfaces/MetaDataExportInterface.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FMetaDataTrait_Base;
template struct TInstancedStruct<FMetaDataTrait_Base>;
#ifdef METADATAPLUGIN_MetaDataExportInterface_generated_h
#error "MetaDataExportInterface.generated.h already included, missing '#pragma once' in MetaDataExportInterface.h"
#endif
#define METADATAPLUGIN_MetaDataExportInterface_generated_h

#define FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual void ExportTraits_Implementation(TArray<TInstancedStruct<FMetaDataTrait_Base> >& OutTraits, bool bClearArray) const {}; \
	DECLARE_FUNCTION(execExportTraits);


#define FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_CALLBACK_WRAPPERS
#define FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	METADATAPLUGIN_API UMetaDataExportInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UMetaDataExportInterface(UMetaDataExportInterface&&); \
	UMetaDataExportInterface(const UMetaDataExportInterface&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(METADATAPLUGIN_API, UMetaDataExportInterface); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMetaDataExportInterface); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UMetaDataExportInterface) \
	METADATAPLUGIN_API virtual ~UMetaDataExportInterface();


#define FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUMetaDataExportInterface(); \
	friend struct Z_Construct_UClass_UMetaDataExportInterface_Statics; \
public: \
	DECLARE_CLASS(UMetaDataExportInterface, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/MetaDataPlugin"), METADATAPLUGIN_API) \
	DECLARE_SERIALIZER(UMetaDataExportInterface)


#define FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_GENERATED_UINTERFACE_BODY() \
	FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~IMetaDataExportInterface() {} \
public: \
	typedef UMetaDataExportInterface UClassType; \
	typedef IMetaDataExportInterface ThisClass; \
	static void Execute_ExportTraits(const UObject* O, TArray<TInstancedStruct<FMetaDataTrait_Base> >& OutTraits, bool bClearArray); \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_14_PROLOG
#define FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_25_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_CALLBACK_WRAPPERS \
	FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h_17_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> METADATAPLUGIN_API UClass* StaticClass<class UMetaDataExportInterface>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_spyde_OneDrive_Documents_Unreal_Projects_PluginDev_Plugins_MetaDataPlugin_Source_MetaDataPlugin_Public_Interfaces_MetaDataExportInterface_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
