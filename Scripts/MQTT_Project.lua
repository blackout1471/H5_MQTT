projLoc = "../"

project "MQTT"
	location(projLoc)
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "mqttpch.h"
	pchsource(projLoc .. "Source/mqttpch.cpp")

	files
	{
		projLoc .. "Source/**.h",	
		projLoc .. "Source/**.cpp"
	}

	includedirs
	{
		projLoc .. "Source"
	}

	filter "system:windows"
		excludes
			{
				projLoc .. "Source/**_Linux.cpp"
			}
		systemversion "latest"
		links {"Ws2_32"}

	filter "system:linux"
		excludes
		{
			projLoc .. "Source/**_Win32.cpp"
		}
		links
		{
			"pthread",
			"uuid"
		}
		
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
