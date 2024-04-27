workspace "NestEngine"
	architecture "x64"
	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "vendor/CHCL/CHCL"
include "vendor/GLFW"
include "vendor/Glad"
include "NestEngine"

project "NestGLSLEmbedder"
	location "NestGLSLEmbedder"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	targetname("nglembed")

	files
	{
		"%{prj.name}/src/*.cpp"
	}

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"GLFW",
		"Glad",
		"opengl32.lib",
		"NestEngine"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"NE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "NE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "NE_RELEASE"
		optimize "On"
