#pragma once

#include "../../Nest/Window.h"

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
		};

		WindowData m_winData;
	};
}