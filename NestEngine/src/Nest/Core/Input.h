#pragma once

#include "Nest/Core/Core.h"
#include "Nest/Core/KeyCodes.h"

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

		inline static bool IsKeyPressed(int keyCode) { return s_instance->isKeyPressedImpl(keyCode); }

		inline static bool IsMouseButtonDown(int button) { return s_instance->isMouseButtonDownImpl(button); }
		inline static float GetMouseX() { return s_instance->getMouseXImpl(); }
		inline static float GetMouseY() { return s_instance->getMouseYImpl(); }
		inline static chcl::Vector2<float> GetMousePos() { return s_instance->getMousePosImpl(); }

	private:
		virtual bool isKeyPressedImpl(int keyCode) = 0;
		
		virtual bool isMouseButtonDownImpl(int button) = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;
		virtual chcl::Vector2<float> getMousePosImpl() = 0;

		static Scope<Input> s_instance;
	};
}