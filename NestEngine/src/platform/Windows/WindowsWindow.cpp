#include "WindowsWindow.h"

#include <iostream>

#include <glad/glad.h>

#include "Nest/Events/WindowEvent.h"
#include "Nest/Events/MouseEvent.h"
#include "Nest/Events/KeyboardEvent.h"

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
			NE_ERROR("[GL ERROR {0}]: {1}", code, message);
		});

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_winData);

		NE_INFO("Created window {0} of size [{1}, {2}].", props.title, props.width, props.height);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NE_ASSERT(status, "GLAD init failed.");

		setVSync(true);

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent closeEvent;
			data.eventCallback(closeEvent);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int, int action, int)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent pressEvent(key, 0);
					data.eventCallback(pressEvent);
					return;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent repeatEvent(key, 1);
					data.eventCallback(repeatEvent);
					return;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent releaseEvent(key);
					data.eventCallback(releaseEvent);
					return;
				}
			}
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent pressEvent(button);
					data.eventCallback(pressEvent);
					return;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent releaseEvent(button);
					data.eventCallback(releaseEvent);
					return;
				}
			}
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double dx, double dy)
		{
			WindowData &data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent moveEvent(dx, dy);
			data.eventCallback(moveEvent);
		});
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