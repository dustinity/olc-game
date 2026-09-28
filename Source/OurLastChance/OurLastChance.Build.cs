// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OurLastChance : ModuleRules
{
	public OurLastChance(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseRTTI = false; // Engine Linux build is compiled with RTTI disabled (WITH_RTTI=0); keep the module in sync or linking fails on missing typeinfo symbols.

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
			"ImageWrapper",
			"NavigationSystem",
			"Niagara",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"AssetRegistry",
			"InputCore",
			"Projects",
		});
	}
}
