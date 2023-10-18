#pragma once

#include <memory>

#include "Core.h"
#include "Window.h"

namespace Nest
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void run();
	protected:
		std::unique_ptr<Window> m_window;
	};

	// To be defined in client
	Application* createApplication();
}