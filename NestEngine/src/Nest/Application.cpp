#include "Application.h"

#include <glad/glad.h>

namespace Nest
{
	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::CreateWindow());
	}

	Application::~Application()
	{

	}

	void Application::run()
	{
		bool running = true;

		while (running)
		{
			glClearColor(0.3f, 0.1f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_window->onUpdate();
		}
	}
}