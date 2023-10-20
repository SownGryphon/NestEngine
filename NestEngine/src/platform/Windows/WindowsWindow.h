#pragma once

#include "Nest/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Nest
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps &props);
		virtual ~WindowsWindow();

		void onUpdate() override;

		unsigned int getWidth() const override;
		unsigned int getHeight() const override;

		inline void setEventCallback(const EventCallbackFn &func) override { m_winData.eventCallback = func; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;
	private:
		void init(const WindowProps &props);
		void shutdown();

		GLFWwindow *m_window;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback;
		};

		WindowData m_winData;
	};
}