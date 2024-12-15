#pragma once

#include <memory>

#include "Nest/Core/Core.h"
#include "Nest/Core/Window.h"
#include "Nest/Core/LayerStack.h"

#include "Nest/Events/Event.h"
#include "Nest/Events/WindowEvent.h"


namespace Nest
{
	class Application
	{
	public:
		Application();
		Application(const std::string &windowTitle, unsigned int windowWidth, unsigned int windowHeight, bool resizable = false);
		virtual ~Application();

		void pushLayer(Layer *layer);

		void run();

		void onEvent(Event &e);
		bool onWindowClose(WindowCloseEvent &e);

		inline Window& getWindow() { return *m_windowHandle; }
		static Application& GetInstance() { return *s_instance; }

	private:
		Scope<Window> m_windowHandle;
		bool m_running = true;
		LayerStack m_layerStack;

		void init();

	private:
		static Application *s_instance;
	};

	// To be defined in client
	Application* createApplication();
}