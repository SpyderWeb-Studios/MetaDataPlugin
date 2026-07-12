using UnrealBuildTool;
 
/*
 *
 * This module is the core editor functionality 
 *
 */

public class MetaDataEditor : ModuleRules
{
	public MetaDataEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd",
			"Slate",
			"SlateCore", "MetaDataEditorCore", "MetaDataIndexingModule"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"MetaDataPlugin",
			"StructUtils",       // Required for FInstancedStruct
			"DataRegistry",      // Required for FDataRegistryId
			"GameplayTags",      // Required for FGameplayTagQuery
            "EditorSubsystem",    // Required for the EditorSubsystem,
            "ToolMenus",
            "Projects",
		});
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

 
		PublicIncludePaths.AddRange(new string[] {"MetaDataEditor/Public"});
		PrivateIncludePaths.AddRange(new string[] {"MetaDataEditor/Private"});
	}
}