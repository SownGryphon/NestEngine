outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["CHCL"] = "../vendor/CHCL/CHCL/src"
IncludeDir["GLFW"] = "../vendor/GLFW/include"
IncludeDir["Glad"] = "../vendor/Glad/include"

project "NestEngine"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"res/shaders/**.glsl*"
	}

	includedirs
	{
		"src",
		"res",
		"%{IncludeDir.CHCL}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
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
		cppdialect "C++20"
		staticruntime "On"
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