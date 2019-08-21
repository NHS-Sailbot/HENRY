include "dep/math"

project "sailbot"
	kind "StaticLib"
	language "C++"
	targetdir("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/build/bin/intermediates/" .. outputdir .. "/%{prj.name}")
	files { "src/**.hpp", "src/**.cpp", "inc/**.hpp" }
	includedirs { "%{inc.sailbot}", "%{prj.location}/dep/math/src" }
	links { "math" }
	warnings "Extra"
	filter "configurations:Debug"
		defines "_CONFIG_DEBUG"
		optimize "Off"
		symbols "On"
	filter "configurations:DebugOptimized"
		defines "_CONFIG_DEBUG_OPTIMIZED"
		optimize "On"
		symbols "Off"
	filter "configurations:Release"
		defines "_CONFIG_RELEASE"
		optimize "On"
		symbols "Off"
	filter "system:windows"
		systemversion "latest"
		defines "_CONFIG_PLATFORM_WINDOWS"
	filter "system:linux"
		defines "_CONFIG_PLATFORM_LINUX"
		links { "opencv_shape", "opencv_stitching", "opencv_superres", 
		"opencv_videostab", "opencv_aruco", "opencv_bgsegm", "opencv_bioinspired", 
		"opencv_ccalib", "opencv_datasets", "opencv_dpm", "opencv_face", "opencv_freetype", 
		"opencv_fuzzy", "opencv_hdf", "opencv_line_descriptor", "opencv_optflow", 
		"opencv_video", "opencv_plot", "opencv_reg", "opencv_saliency", "opencv_stereo", 
		"opencv_structured_light", "opencv_phase_unwrapping", "opencv_rgbd", "opencv_viz", 
		"opencv_surface_matching", "opencv_text", "opencv_ximgproc", "opencv_calib3d", 
		"opencv_features2d", "opencv_flann", "opencv_xobjdetect", "opencv_objdetect", 
		"opencv_ml", "opencv_xphoto", "opencv_highgui", "opencv_videoio", "opencv_imgcodecs", 
		"opencv_photo", "opencv_imgproc", "opencv_core" }
	filter "system:macosx"
		defines "_CONFIG_PLATFORM_MACOS"
