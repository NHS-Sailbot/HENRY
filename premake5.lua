workspace "HENRY"
	architecture "x64"
	configurations { "Debug", "DebugOptimized", "Release" }
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
inc = {}
inc["sailbot"] = "%{wks.location}/sailbot/src"

startproject "main"
include "sailbot"
include "main"
include "test"
