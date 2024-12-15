outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.c",
		"include/glad/**.h",
		"include/KHR/**.h"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"