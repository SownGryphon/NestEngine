#include "WindowsWindow.h"

#include <iostream>

#include <glad/glad.h>

namespace Nest
{
	static bool s_GLFWInitialized = false;

	WindowsWindow::WindowsWindow(const WindowProps &props)
	{
		init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Window::~Window();
		shutdown();
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	unsigned int WindowsWindow::getWidth() const
	{
		return m_winData.width;
	}

	unsigned int WindowsWindow::getHeight() const
	{
		return m_winData.height;
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_winData.vSync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_winData.vSync;
	}

	void WindowsWindow::init(const WindowProps &props)
	{
		m_winData.width = props.width;
		m_winData.height = props.height;
		m_winData.title = props.title;

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			NE_ASSERT(success, "GLFW init failed.");

			s_GLFWInitialized = true;
		}

		glfwSetErrorCallback([](int code, const char *message) {
			std::cout << "[GL ERROR " << code << "]: " << message << std::endl;
		});

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_winData);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NE_ASSERT(status, "GLAD init failed.");

		setVSync(true);
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	Window* Window::CreateWindow(const WindowProps &props)
	{
		return new WindowsWindow(props);
	}
}