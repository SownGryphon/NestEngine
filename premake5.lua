workspace "NestEngine"
	architecture "x64"
	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "vendor/CHCL/CHCL"
include "vendor/Glad"
include "vendor/GLFW"
include "NestEngine"

project "NestGLSLEmbedder"
	location "NestGLSLEmbedder"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	targetname("nglembed")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/*.cpp"
	}
	
	includedirs
	{
		"vendor/CHCL/CHCL/src",
	}
	
	links
	{
		"CHCL"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/res/shaders/**.glsl*"
	}

	includedirs
	{
		"vendor/CHCL/CHCL/src",
		"vendor/GLFW/include",
		"vendor/Glad/include",
		"NestEngine/src"
	}

	links
	{
		"CHCL",
		"Glad",
		"opengl32.lib",
		"NestEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"NE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines { "NE_DEBUG", "NE_ENABLE_ASSERTS" }
		symbols "On"

	filter "configurations:Release"
		defines "NE_RELEASE"
		optimize "On"
