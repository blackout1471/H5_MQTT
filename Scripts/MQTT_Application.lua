projLoc = "../Application/"

project "MQTTApplication"
	location(projLoc)
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		projLoc .. "**.cpp"
	}

	includedirs
	{
		projLoc .. "../Source"
	}

	links
	{
		"MQTT"
	}

	filter "system:windows"
		links {"Ws2_32"}

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
