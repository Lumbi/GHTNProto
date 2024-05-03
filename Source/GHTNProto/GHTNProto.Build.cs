// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class GHTNProto : ModuleRules
{
	public GHTNProto(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] { "Kismet" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });

        // GHTN Library

        string GHTNLibraryIncludePath = Path.Combine(ModuleDirectory, "ThirdParty", "GHTN", "include");
        PublicIncludePaths.Add(GHTNLibraryIncludePath);
        string GHTNLibraryPath = Path.Combine(ModuleDirectory, "ThirdParty", "GHTN", "x64", "Release", "GHTN.lib");
        PublicAdditionalLibraries.Add(GHTNLibraryPath);

        System.Console.WriteLine("TEST: GHTNLibraryIncludePath = " + GHTNLibraryIncludePath);
        System.Console.WriteLine("TEST: GHTNLibraryPath = " + GHTNLibraryPath);
    }
}
