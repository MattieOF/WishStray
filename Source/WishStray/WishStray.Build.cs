// copyright lololol

using UnrealBuildTool;

public class WishStray : ModuleRules
{
	public WishStray(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GeometryCollectionEngine" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}