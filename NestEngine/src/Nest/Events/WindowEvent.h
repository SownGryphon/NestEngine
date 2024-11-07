#pragma once

#include <geometry/Vector2.h>

#include "Event.h"

namespace Nest
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryWindow);
	};

	class WindowResizeEvent : public Event
	{
		unsigned int m_newWidth, m_newHeight;
	public:
		WindowResizeEvent() : m_newWidth(0), m_newHeight(0) {}
		WindowResizeEvent(unsigned int newWidth, unsigned int newHeight) : m_newWidth(newWidth), m_newHeight(newHeight) {}

		inline unsigned int getWidth() const { return m_newWidth; }
		inline unsigned int getHeight() const { return m_newHeight; }
		inline chcl::Vector2<unsigned int> getSize() const { return { m_newWidth, m_newHeight }; }

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryWindow)
	};
}