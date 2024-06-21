#pragma once

#include "Nest/Core/Core.h"
#include "Nest/Core/KeyCodes.h"
#include "Nest/Events/Event.h"

#include "geometry/Vector2.h"

namespace Nest
{
	class Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool OnEvent(Event &e) { return s_instance->onEventImpl(e); }
		inline static void Clear() { s_instance->clearImpl(); }

		inline static bool WasKeyPressed(int keyCode) { return s_instance->wasKeyPressedImpl(keyCode); }
		inline static bool IsKeyDown(int keyCode) { return s_instance->isKeyDownImpl(keyCode); }

		inline static bool IsCursorCaptured() { s_instance->isCursorCapturedImpl(); }
		inline static void CaptureCursor(bool capture) { s_instance->captureCursorImpl(capture); }

		inline static bool WasMouseButtonPressed(int button) { return s_instance->wasMouseButtonPressedImpl(button); }
		inline static bool IsMouseButtonDown(int button) { return s_instance->isMouseButtonDownImpl(button); }
		inline static float GetMouseScroll() { return s_instance->getMouseScrollImpl(); }
		inline static float GetCursorX() { return s_instance->getCursorXImpl(); }
		inline static float GetCursorY() { return s_instance->getCursorYImpl(); }
		inline static chcl::Vector2<float> GetCursorPos() { return s_instance->getCursorPosImpl(); }
		inline static chcl::Vector2<float> GetCursorMovement() { return s_instance->getCursorMovementImpl(); }

	protected:
		virtual bool onEventImpl(Event &e) = 0;
		virtual void clearImpl() = 0;

		virtual bool wasKeyPressedImpl(int keyCode) = 0;
		virtual bool isKeyDownImpl(int keyCode) = 0;
		
		virtual bool isCursorCapturedImpl() = 0;
		virtual void captureCursorImpl(bool capture) = 0;

		virtual bool wasMouseButtonPressedImpl(int button) = 0;
		virtual bool isMouseButtonDownImpl(int button) = 0;
		virtual float getMouseScrollImpl() = 0;
		virtual float getCursorXImpl() = 0;
		virtual float getCursorYImpl() = 0;
		virtual chcl::Vector2<float> getCursorPosImpl() = 0;
		virtual chcl::Vector2<float> getCursorMovementImpl() = 0;

		static Scope<Input> s_instance;
	};
}