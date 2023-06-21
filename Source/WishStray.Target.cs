// copyright lololol

using UnrealBuildTool;
using System.Collections.Generic;

public class WishStrayTarget : TargetRules
{
	public WishStrayTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "WishStray" } );
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
	}
}
