using UnrealBuildTool;
using System.Collections.Generic;

public class ArenaClashServerTarget : TargetRules
{
	public ArenaClashServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("ArenaClash");
	}
}
