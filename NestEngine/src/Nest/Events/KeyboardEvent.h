#pragma once

#include "Event.h"

namespace Nest
{
	class KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		KeyEvent(int keyCode)
			: m_keyCode(keyCode) {}

		int m_keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, unsigned int repeatCount)
			: KeyEvent(keyCode), m_repeatCount(repeatCount) {}

		inline unsigned int getRepeatCount() const { return m_repeatCount; }

		EVENT_CLASS_TYPE(KeyPressed);

	private:
		unsigned int m_repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		EVENT_CLASS_TYPE(KeyReleased);
	};
}