solLoc = "../"

workspace "H5MQTT"
	location(solLoc)
	
	startproject "MQTT"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

group "Test"
	include "MQTT_UnitTest.lua"
group ""

include "MQTT_Project.lua"



