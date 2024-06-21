#pragma once

#include <chrono>

#include "Nest/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Nest
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps &props);
		virtual ~WindowsWindow();

		void processEvents() override;
		void onUpdate() override;

		unsigned int getWidth() const override;
		unsigned int getHeight() const override;

		inline void setEventCallback(const EventCallbackFn &func) override { m_winData.eventCallback = func; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;
		virtual void setFPS(float fps) override;
		inline void* getNativeWindow() override { return m_window; }

	private:
		void init(const WindowProps &props);
		void shutdown();

		GLFWwindow *m_window;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool resizable;
			bool vSync = false;
			float fps = 0.f;
			std::chrono::high_resolution_clock::time_point lastRefresh;

			EventCallbackFn eventCallback;
		};

		WindowData m_winData;
	};
}