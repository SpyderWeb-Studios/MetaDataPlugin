using UnrealBuildTool;
 
public class MetaDataEditor : ModuleRules
{
	public MetaDataEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UnrealEd"});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"MetaDataPlugin",
			"StructUtils",       // Required for FInstancedStruct
			"DataRegistry",      // Required for FDataRegistryId
			"GameplayTags",      // Required for FGameplayTagQuery
            "EditorSubsystem"    // Required for the EditorSubsystem
		});
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

 
		PublicIncludePaths.AddRange(new string[] {"MetaDataEditor/Public"});
		PrivateIncludePaths.AddRange(new string[] {"MetaDataEditor/Private"});
	}
}