// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class GHTNPluginLibrary : ModuleRules
{
	public GHTNPluginLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		/*
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "GHTN.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			// PublicDelayLoadDLLs.Add("GHTN.dll");

			// Ensure that the DLL is staged along with the executable
			// RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/GHTNPluginLibrary/Win64/ExampleLibrary.dll");
        }
		*/
	}
}
