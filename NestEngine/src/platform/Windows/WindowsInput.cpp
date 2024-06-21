#include "WindowsInput.h"

#include "Nest/Core/Application.h"

Nest::Scope<Nest::Input> Nest::Input::s_instance = createScope<WindowsInput>();

bool Nest::WindowsInput::onEventImpl(Event &e)
{
	EventDispatcher dispatcher(e);
	dispatcher.dispatchEvent<KeyPressedEvent>(NE_BIND_EVENT_FN(WindowsInput::onKeyPressEvent));
	dispatcher.dispatchEvent<MouseButtonPressedEvent>(NE_BIND_EVENT_FN(WindowsInput::onMousePressEvent));
	dispatcher.dispatchEvent<MouseScrolledEvent>(NE_BIND_EVENT_FN(WindowsInput::onMouseScrollEvent));
	return false;
}

void Nest::WindowsInput::clearImpl()
{
	m_keysPressed.clear();
	m_mouseButtonsPressed.clear();
	m_lastCursorPos = getCursorPosImpl();
	m_mouseScroll = 0.f;
}

bool Nest::WindowsInput::isKeyDownImpl(int keyCode)
{
	GLFWwindow* window = getWindow();
	auto state = glfwGetKey(window, keyCode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

void Nest::WindowsInput::captureCursorImpl(bool capture)
{
	GLFWwindow* window = getWindow();
	glfwSetInputMode(window, GLFW_CURSOR, capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, capture ? GLFW_TRUE : GLFW_FALSE);

	m_cursorCaptured = capture;
}

bool Nest::WindowsInput::isMouseButtonDownImpl(int button)
{
	GLFWwindow* window = getWindow();
	auto buttonState = glfwGetMouseButton(window, button);
	return buttonState == GLFW_PRESS;
}

chcl::Vector2<float> Nest::WindowsInput::getCursorPosImpl()
{
	GLFWwindow* window = getWindow();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return chcl::Vector2<float>((float)xpos, (float)ypos);
}

GLFWwindow* Nest::WindowsInput::getWindow()
{
	return static_cast<GLFWwindow*>(Application::GetInstance().getWindow().getNativeWindow());
}

bool Nest::WindowsInput::onKeyPressEvent(KeyPressedEvent &e)
{
	if (e.getRepeatCount() == 0)
		m_keysPressed[e.getKeyCode()] = true;
	return false;
}

bool Nest::WindowsInput::onMousePressEvent(MouseButtonPressedEvent &e)
{
	m_mouseButtonsPressed[e.getButton()] = true;
	return false;
}

bool Nest::WindowsInput::onMouseScrollEvent(MouseScrolledEvent &e)
{
	m_mouseScroll += e.getDY();
	return false;
}