// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OurLastChanceTarget : TargetRules
{
	public OurLastChanceTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("OurLastChance");

		// See OurLastChanceEditor.Target.cs -- same Smart App Control
		// signing workaround. Development Game builds launched via
		// UnrealEditor.exe -game load this same DLL.
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PostBuildSteps.Add(
				"\"C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.26100.0\\x64\\signtool.exe\" sign /n \"OurLastChanceDevSigning\" /fd SHA256 \"$(ProjectDir)\\Binaries\\Win64\\UnrealEditor-OurLastChance.dll\" & exit /b 0");
		}
	}
}
