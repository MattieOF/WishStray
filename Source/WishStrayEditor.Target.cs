// copyright lololol

using UnrealBuildTool;
using System.Collections.Generic;

public class WishStrayEditorTarget : TargetRules
{
	public WishStrayEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "WishStray" } );
	}
}
