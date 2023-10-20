#pragma once

#include "Event.h"

namespace Nest
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float dx, float dy)
			: m_dx(dx), m_dy(dy) {}

		inline float getHorizontal() const { return m_dx; }
		inline float getVertical() const { return m_dx; }

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);

	private:
		float m_dx, m_dy;
	};

	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(int button)
			: m_button(button) {}

		inline int getButton() const { return m_button; }

		EVENT_CLASS_TYPE(MouseButtonPressed);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);

	private:
		int m_button;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(int button)
			: m_button(button) {}

		inline int getButton() const { return m_button; }

		EVENT_CLASS_TYPE(MouseButtonReleased);
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse);

	private:
		int m_button;
	};
}