// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OurLastChance : ModuleRules
{
	public OurLastChance(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = true;

		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"UMG",
			"EnhancedInput",
			"CommonUI",
			"ProceduralMeshComponent",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"InputCore",
			"Projects",
		});
	}
}
