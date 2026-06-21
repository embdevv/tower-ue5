using UnrealBuildTool;

public class ENG02_Tower : ModuleRules
{
    public ENG02_Tower(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG"
        });

        // Supports raw Slate UI underneath UMG
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}