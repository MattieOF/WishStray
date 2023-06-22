// copyright lololol

using UnrealBuildTool;

public class WishStray : ModuleRules
{
	public WishStray(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GeometryCollectionEngine", "ChaosSolverEngine", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		if (Target.bBuildEditor)
			PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
	}
}
