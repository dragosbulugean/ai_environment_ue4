// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;
using System;

public class kortex_environment : ModuleRules
{
    public kortex_environment(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets","Networking"});
        
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

        else if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
        {
            Console.WriteLine("DEBUG : " + ThirdPartyModulePath + @"\ThirdParty\lib\libzmq-v120-mt-4_0_4.lib");
            PublicIncludePaths.Add(ThirdPartyModulePath + @"\ThirdParty\include");

            // OpenCV
            //string openCv = ThirdPartyModulePath + @"\ThirdParty\lib\opencv_ts300.lib";
            //PublicAdditionalLibraries.Add(openCv);
            //string openCv2 = ThirdPartyModulePath + @"\ThirdParty\lib\opencv_world300.lib";
            //PublicAdditionalLibraries.Add(openCv2);

             //ZeroMQ
            //RuntimeDependencies.Add(new RuntimeDependency(ThirdPartyModulePath + @"\ThirdParty\bin\libzmq-v120-mt-4_0_4.dll"));
            string zmq = ThirdPartyModulePath + @"\ThirdParty\lib\libzmq-v120-mt-4_0_4.lib";
            PublicAdditionalLibraries.Add(zmq);
            
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
