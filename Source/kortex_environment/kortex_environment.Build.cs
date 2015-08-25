// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;
using System;

public class kortex_environment : ModuleRules
{
    public kortex_environment(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        
        PrivateDependencyModuleNames.AddRange (new string[] { });
        
        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            // Add include folder
            PublicIncludePaths.Add(ThirdPartyModulePath + @"/ThirdParty/include");
            PublicIncludePaths.Add("/usr/local/include");
            // Add public libs
            string libPng = ThirdPartyModulePath + @"/ThirdParty/lib/libpng15.15.dylib";
            PublicAdditionalLibraries.Add(libPng);
        }
        
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        
        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //		if (UEBuildConfiguration.bCompileSteamOSS == true)
        //		{
        //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //		}
        // }
    }
    
    
    
    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }
    private string ThirdPartyModulePath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "..")); }
    }
    }
