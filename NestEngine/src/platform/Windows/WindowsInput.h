#pragma once

#include <unordered_map>

#include "Nest/Core/Core.h"
#include "Nest/Core/Input.h"

#include "Nest/Events/KeyboardEvent.h"
#include "Nest/Events/MouseEvent.h"

namespace Nest
{
	class WindowsInput : public Input
	{
	protected:
		std::unordered_map<int, bool> m_keysPressed, m_mouseButtonsPressed;
		bool m_cursorCaptured = false;
		chcl::Vector2<float> m_lastCursorPos;
		float m_mouseScroll;

		virtual bool onEventImpl(Event &e) override;
		virtual void clearImpl() override;

		inline virtual bool wasKeyPressedImpl(int keyCode) override { return m_keysPressed[keyCode]; }
		virtual bool isKeyDownImpl(int keyCode) override;

		inline virtual bool isCursorCapturedImpl() override { return m_cursorCaptured; }
		virtual void captureCursorImpl(bool capture) override;

		inline virtual bool wasMouseButtonPressedImpl(int button) override { return m_mouseButtonsPressed[button]; }
		virtual bool isMouseButtonDownImpl(int button) override;
		virtual float getMouseScrollImpl() override { return m_mouseScroll; }
		inline virtual float getCursorXImpl() override { return getCursorPosImpl().x; }
		inline virtual float getCursorYImpl() override { return getCursorPosImpl().y; }
		virtual chcl::Vector2<float> getCursorPosImpl() override;
		inline virtual chcl::Vector2<float> getCursorMovementImpl() override { return getCursorPosImpl() - m_lastCursorPos; }

		//GLFWwindow* getWindow();

		bool onKeyPressEvent(KeyPressedEvent &e);
		bool onMousePressEvent(MouseButtonPressedEvent &e);
		bool onMouseScrollEvent(MouseScrolledEvent &e);
	};
}