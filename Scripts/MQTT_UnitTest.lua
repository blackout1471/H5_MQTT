projLoc = "../Tests/Unit"

project "UnitTest"
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
		projLoc .. "/**.cpp"
	}

	IncludeDir = {}
	IncludeDir["gtest"] = "../vendor/googletest/googletest/include/"
	IncludeDir["Source"] = "../Source/"

	includedirs
	{
		projLoc,
		"%{IncludeDir.gtest}",
		"%{IncludeDir.Source}"
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
			"pthread"
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
