solLoc = "../"

workspace "H5MQTT"
	location(solLoc)
	
	startproject "MQTTApplication"

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
	include "gtest.lua"
	include "MQTT_UnitTest.lua"
	include "MQTT_IntegrationTest.lua"
group ""

include "MQTT_Project.lua"
include "MQTT_Application.lua"
