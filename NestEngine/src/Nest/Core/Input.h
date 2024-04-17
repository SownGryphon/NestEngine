#pragma once

#include "Nest/Core/Core.h"
#include "Nest/Core/KeyCodes.h"

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

	private:
		virtual bool isKeyPressedImpl(int keyCode) = 0;

		static Scope<Input> s_instance;
	};
}