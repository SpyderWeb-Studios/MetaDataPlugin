#pragma once
#include "Subsystems/MetaDataEditorSubsystem.h"


struct FMetaScopedBakeSession
{
public:
	FMetaScopedBakeSession()
	{
		// On creation, increment the depth. 
		// If it was 0, this is the outermost scope, so we can initialize the transaction/UI here if needed.
		if (UMetaDataEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>())
		{
			Subsystem->BeginBakeSession();
		}
	}

	~FMetaScopedBakeSession()
	{
		// When this struct is destroyed (end of function), decrement the depth.
		// If depth reaches 0, the subsystem automatically calls Flush() on the providers.
		if (UMetaDataEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>())
		{
			Subsystem->EndBakeSession();
		}
	}
};