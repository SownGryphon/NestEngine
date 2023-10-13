#pragma once

#ifdef NT_PLATFORM_WINDOWS
	#ifdef NT_BUILD_DLL
		#define NEST_API __declspec(dllexport)
	#else
		#define NEST_API __declspec(dllexport)
	#endif
#else
	#error Nest only supports Windows
#endif
