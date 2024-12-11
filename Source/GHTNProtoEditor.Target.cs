// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GHTNProtoEditorTarget : TargetRules
{
	public GHTNProtoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
		CppStandard = CppStandardVersion.Cpp20;
		bOverrideBuildEnvironment = true;
		bUseAutoRTFMCompiler = false;
        ExtraModuleNames.Add("GHTNProto");
        GlobalDefinitions.Add("FORCE_ANSI_ALLOCATOR=1");
    }
}
