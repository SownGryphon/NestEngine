#include "Application.h"

#include <chrono>

#include <glad/glad.h>

#include "Nest/Core/Core.h"

#include "Nest/Renderer/GraphicsAPI.h"

#include "Nest/renderer/Renderer.h"
#include "Nest/Utils/Time.h"
#include "Nest/Core/Input.h"

namespace Nest
{
	Application *Application::s_instance;

	Application::Application()
	{
		m_windowHandle = Scope<Window>(Window::Create());
		init();
	}

	Application::Application(const std::string &windowTitle, unsigned int windowWidth, unsigned int windowHeight, bool resizable)
	{
		m_windowHandle = Scope<Window>(Window::Create({ windowTitle, windowWidth, windowHeight, resizable }));
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
		std::chrono::milliseconds lastStepTime = Time::getTimeMillis();

		while (m_running)
		{
			auto now = Time::getTimeMillis();

			m_windowHandle->processEvents();

			for (Layer *layer : m_layerStack)
				layer->onUpdate(now - lastStepTime);

			m_windowHandle->onUpdate();

			Input::Clear();

			lastStepTime = now;
		}
	}

	void Application::onEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatchEvent<WindowCloseEvent>(NE_BIND_EVENT_FN(Application::onWindowClose));

		Input::OnEvent(e);

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
		m_windowHandle->setEventCallback(NE_BIND_EVENT_FN(Application::onEvent));
		NE_ASSERT(!s_instance, "Can only have one application instance.");
		s_instance = this;

		Renderer::init(m_windowHandle->getWidth(), m_windowHandle->getHeight());
	}
}