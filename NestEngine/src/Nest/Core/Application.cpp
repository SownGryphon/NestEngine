#include "Application.h"

#include <glad/glad.h>

#include "Nest/Graphics/Renderer.h"

namespace Nest
{
	Application *Application::s_instance;

	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::CreateWindow());
		init();
	}

	Application::Application(const std::string & windowTitle, unsigned int windowWidth, unsigned int windowHeight)
	{
		m_window = std::unique_ptr<Window>(Window::CreateWindow({ windowTitle, windowWidth, windowHeight }));
		init();
	}

	Application::~Application()
	{
		Renderer::shutdown();
	}

	void Application::pushLayer(Layer *layer)
	{
		m_layerStack.pushLayer(layer);
	}

	void Application::run()
	{
		while (m_running)
		{
			for (Layer *layer : m_layerStack)
				layer->onUpdate();

			m_window->onUpdate();
		}
	}

	void Application::onEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatchEvent<WindowCloseEvent>(NE_BIND_EVENT_FN(Application::onWindowClose));

		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
		{
			(*it)->onEvent(e);
			if (e.handled)
				break;
		}
	}

	bool Application::onWindowClose(WindowCloseEvent &e)
	{
		m_running = false;
		return true;
	}

	void Application::init()
	{
		m_window->setEventCallback(NE_BIND_EVENT_FN(Application::onEvent));
		NE_ASSERT(!s_instance, "Can only have one application instance.");
		s_instance = this;

		Renderer::init();
	}
}