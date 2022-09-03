projLoc = "../Tests/IntegrationTest"

project "IntegrationTest"
	location(projLoc)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		projLoc .. "/**.h",	
		projLoc .. "/**.cpp",
		"../vendor/MQTT-C/include/**.h",
		"../vendor/MQTT-C/src/**.c"
	}

	IncludeDir = {}
	IncludeDir["gtest"] = "../vendor/googletest/googletest/include/"
	IncludeDir["gmock"] = "../vendor/googletest/googlemock/include/"
	IncludeDir["mqttClient"] = "../vendor/MQTT-C/include/"
	IncludeDir["Source"] = "../Source/"

	includedirs
	{
		projLoc,
		"%{IncludeDir.gtest}",
		"%{IncludeDir.Source}",
		"%{IncludeDir.gmock}",
		"%{IncludeDir.mqttClient}"
	}

	links
	{
		"gtest",
		"MQTT"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
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
