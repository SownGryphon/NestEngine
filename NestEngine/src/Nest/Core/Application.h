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
		Application(const std::string &windowTitle, unsigned int windowWidth, unsigned int windowHeight);
		virtual ~Application();

		void pushLayer(Layer *layer);

		void run();

		void onEvent(Event &e);
		bool onWindowClose(WindowCloseEvent &e);

		inline Window& getWindow() { return *m_window; }

	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layerStack;

		void init();

	private:
		static Application *s_instance;
	};

	// To be defined in client
	Application* createApplication();
}