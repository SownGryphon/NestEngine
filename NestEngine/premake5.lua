outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NestEngine"
	location "NestEngine"
	kind "StaticLib"
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
		"%{prj.name}/src",
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