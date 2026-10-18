// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArenaClash : ModuleRules
{
	public ArenaClash(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ArenaClash",
			"ArenaClash/Variant_Platforming",
			"ArenaClash/Variant_Platforming/Animation",
			"ArenaClash/Variant_Combat",
			"ArenaClash/Variant_Combat/AI",
			"ArenaClash/Variant_Combat/Animation",
			"ArenaClash/Variant_Combat/Gameplay",
			"ArenaClash/Variant_Combat/Interfaces",
			"ArenaClash/Variant_Combat/UI",
			"ArenaClash/Variant_SideScrolling",
			"ArenaClash/Variant_SideScrolling/AI",
			"ArenaClash/Variant_SideScrolling/Gameplay",
			"ArenaClash/Variant_SideScrolling/Interfaces",
			"ArenaClash/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
