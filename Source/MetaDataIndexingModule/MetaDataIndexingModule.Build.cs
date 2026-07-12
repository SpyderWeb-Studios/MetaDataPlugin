using UnrealBuildTool;



public class MetaDataIndexingModule : ModuleRules
{
    public MetaDataIndexingModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "UnrealEd",
                "EditorSubsystem",
                "MetaDataEditorCore",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "JSON", "JsonUtilities", 
                "Projects", "EditorScriptingUtilities",
            }
        );
    }
}