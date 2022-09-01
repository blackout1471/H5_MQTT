project "gtest"
	location "../vendor/googletest/googletest"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
        cppdialect "C++17"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../vendor/googletest/googletest/**.h",
        "../vendor/googletest/googletest/**.hpp",
        "../vendor/googletest/googletest/src/gtest-all.cc",
    }

    includedirs
    {
	"../vendor/googletest/googletest/include",
	"../vendor/googletest/googletest/"
    }

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
