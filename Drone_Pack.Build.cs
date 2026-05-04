using UnrealBuildTool;

public class Drone_Pack : ModuleRules
{
    public Drone_Pack(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "ProceduralMeshComponent"
            });

        PrivateDependencyModuleNames.AddRange(
            new string[] { });
    }
}