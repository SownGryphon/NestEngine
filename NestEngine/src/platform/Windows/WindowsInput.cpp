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
	return false;
}

void Nest::WindowsInput::captureCursorImpl(bool capture)
{
	m_cursorCaptured = capture;
}

bool Nest::WindowsInput::isMouseButtonDownImpl(int button)
{
	return false;
}

chcl::Vector2<float> Nest::WindowsInput::getCursorPosImpl()
{
	return { 0.f, 0.f };
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