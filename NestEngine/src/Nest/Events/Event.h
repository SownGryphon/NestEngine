#pragma once

#include "Nest/Core/Core.h"

namespace Nest
{
	enum class EventType
	{
		None = 0,
		WindowClose,
		KeyPressed, KeyReleased,
		MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryWindow			= BIT(1),
		EventCategoryInput			= BIT(2),
		EventCategoryKeyboard		= BIT(3),
		EventCategoryMouse			= BIT(4),
		EventCategoryMouseButton	= BIT(5)
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
									virtual EventType getEventType() const override { return GetStaticType(); }

	#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class Event
	{
	public:
		bool handled = false;

		virtual EventType getEventType() const = 0;
		virtual int getCategoryFlags() const = 0;

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event &e)
			: m_event(e) {}

		template <typename T, typename F>
		bool dispatchEvent(const F &func)
		{
			if (m_event.getEventType() == T::GetStaticType())
			{
				m_event.handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}

	private:
		Event &m_event;
	};
}