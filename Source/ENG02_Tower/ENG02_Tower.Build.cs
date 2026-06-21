using UnrealBuildTool;

public class ENG02_Tower : ModuleRules
{
    public ENG02_Tower(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

   
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

        // added this line to support raw UI systems underneath UMG
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}