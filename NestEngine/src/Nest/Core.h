#pragma once

#ifdef NE_PLATFORM_WINDOWS
	#ifdef NE_BUILD_DLL
		#define NEST_API __declspec(dllexport)
	#else
		#define NEST_API __declspec(dllexport)
	#endif
#else
	#error Nest only supports Windows
#endif
