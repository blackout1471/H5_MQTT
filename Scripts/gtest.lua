project "gtest"
	location "../vendor/gtest"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	runtime "Debug"

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