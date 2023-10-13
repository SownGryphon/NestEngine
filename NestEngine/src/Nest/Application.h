#pragma once

#include "Core.h"

namespace Nest
{
	class NEST_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// To be defined in client
	Application* createApplication();
}