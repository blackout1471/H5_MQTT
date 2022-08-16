workspace "H5MQTT"
	location "../"
	
	startproject "MQTT"

	configurations
	{
		"Debug",
		"Release",
		"Distribution",
		"Debug_ARM64",
		"Release_ARM64",
		"Distribution_ARM64"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "MQTT"
	location "../"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "../Source/mqttpch.h"
	pchsource "../Source/mqttpch.cpp"

	files
	{
		"../Source/**.h",	
		"../Source/**.cpp"
	}
	

	filter "system:windows"
		systemversion "latest"
		architecture "x86_64"

	filter "configurations:*ARM"
		architecture "ARM64"
		
	filter "configurations:Debug"
		defines "_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "_DIST"
		runtime "Release"
		optimize "on"