#pragma once

#include "Nest/Core/Core.h"

namespace Nest
{
	class Platform
	{
	public:
		virtual void init() = 0;
		
		static Nest::Ref<Nest::Platform> s_nestPlatform;
	};
}