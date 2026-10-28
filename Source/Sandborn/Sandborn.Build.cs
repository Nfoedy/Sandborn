// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Sandborn : ModuleRules
{
	public Sandborn(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Sandborn",
			"Sandborn/Variant_Platforming",
			"Sandborn/Variant_Platforming/Animation",
			"Sandborn/Variant_Combat",
			"Sandborn/Variant_Combat/AI",
			"Sandborn/Variant_Combat/Animation",
			"Sandborn/Variant_Combat/Gameplay",
			"Sandborn/Variant_Combat/Interfaces",
			"Sandborn/Variant_Combat/UI",
			"Sandborn/Variant_SideScrolling",
			"Sandborn/Variant_SideScrolling/AI",
			"Sandborn/Variant_SideScrolling/Gameplay",
			"Sandborn/Variant_SideScrolling/Interfaces",
			"Sandborn/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
