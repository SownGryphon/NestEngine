outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NestEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"res/shaders/**.glsl*",
		"res/embeddedShaders/**.h"
	}

	includedirs
	{
		"src",
		"res",
		"../vendor/CHCL/CHCL/src",
		"../vendor/GLFW/include",
		"../vendor/Glad/include"
	}

	links
	{
		"CHCL",
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	defines
	{
		"GLFW_INCLUDE_NONE"
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