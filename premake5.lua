workspace "HENRY"
	configurations { "Debug", "DebugOptimized", "Release", "ARM" }
	architecture "x64"
	filter "configurations:ARM"
		architecture "ARM"
	filter {}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	inc = {}
	inc["sailbot"] = "%{wks.location}/sailbot/src"

	startproject "test"
	include "sailbot"
	include "test"
	include "main"

    flags { "MultiProcessorCompile" }
    filter "configurations:DebugOptimized"
        flags { "LinkTimeOptimization" }
    filter "configurations:Release"
        flags { "LinkTimeOptimization" }
    