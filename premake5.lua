workspace "HENRY"
	configurations { "Debug", "DebugOptimized", "Release", "ARM" }
	architecture "x64"
	filter "configurations:ARM"
		architecture "ARM"
	filter {}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	inc = {}
	inc["sailbot"] = "%{wks.location}/sailbot/src"
	inc["glfw"] = "%{wks.location}/glfw/include"
	inc["glad"] = "%{wks.location}/glad/include"

	startproject "main"
	include "sailbot"
	include "glfw"
	include "glad"
	include "test"
	include "main"

