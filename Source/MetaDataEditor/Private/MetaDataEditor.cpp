

#include "MetaDataEditor/Public/MetaDataEditor.h"

DEFINE_LOG_CATEGORY(MetaDataEditor);

#define LOCTEXT_NAMESPACE "FMetaDataEditor"

void FMetaDataEditor::StartupModule()
{
	UE_LOG(MetaDataEditor, Warning, TEXT("MetaDataEditor module has been loaded"));
}

void FMetaDataEditor::ShutdownModule()
{
	UE_LOG(MetaDataEditor, Warning, TEXT("MetaDataEditor module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMetaDataEditor, MetaDataEditor)