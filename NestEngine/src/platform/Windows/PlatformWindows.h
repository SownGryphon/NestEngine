#pragma once

#ifdef NE_PLATFORM_WINDOWS

#include "Nest/Core/Platform.h"
#include "platform/Windows/CleanWindows.h"

namespace Nest
{
	class PlatformWindows : public Nest::Platform
	{
	public:
		virtual void init() override;

		inline static HINSTANCE s_instanceHandle;
	};
}

#endif // NE_PLATFORM_WINDOWS
