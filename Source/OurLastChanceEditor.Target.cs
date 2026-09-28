// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OurLastChanceEditorTarget : TargetRules
{
	public OurLastChanceEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("OurLastChance");

		// Windows Smart App Control (enforcing on this dev machine) blocks
		// loading the locally-built, unsigned UnrealEditor-OurLastChance.dll.
		// Auto-signs it with a local dev cert ("OurLastChanceDevSigning") on
		// every build if that cert is installed in the current user's store
		// (see the project's Smart App Control setup notes for how to create
		// and trust it). Always exits 0 -- never fails the build -- so this
		// silently no-ops on machines without the cert, including CI.
		// Only meaningful on Windows (signtool + Smart App Control); skip on
		// Linux/macOS where the step would fail the build.
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PostBuildSteps.Add(
				"\"C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.26100.0\\x64\\signtool.exe\" sign /n \"OurLastChanceDevSigning\" /fd SHA256 \"$(ProjectDir)\\Binaries\\Win64\\UnrealEditor-OurLastChance.dll\" & exit /b 0");
		}
	}
}
