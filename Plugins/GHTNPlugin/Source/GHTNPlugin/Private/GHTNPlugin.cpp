// Copyright Epic Games, Inc. All Rights Reserved.

#include "GHTNPlugin.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "GHTNPluginLibrary/ExampleLibrary.h"

#define LOCTEXT_NAMESPACE "FGHTNPluginModule"

void FGHTNPluginModule::StartupModule()
{
	/*
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("GHTNPlugin")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
// #if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/GHTNPluginLibrary/Win64/ExampleLibrary.dll"));
// #endif // PLATFORM_WINDOWS

//	ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

//  if (ExampleLibraryHandle)
	{
		// Call the test function in the third party library that opens a message box
		ExampleLibraryFunction();
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
	*/
}

void FGHTNPluginModule::ShutdownModule()
{
	/*
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
	*/
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGHTNPluginModule, GHTNPlugin)
