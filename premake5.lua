workspace "HENRY"
	architecture "x64"
	configurations { "Debug", "DebugOptimized", "Release" }
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
inc = {}
inc["sailbot"] = "%{wks.location}/sailbot/src"

startproject "testing"
include "sailbot"
include "testing"
