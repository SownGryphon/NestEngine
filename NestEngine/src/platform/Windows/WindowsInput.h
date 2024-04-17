#pragma once

#include "Nest/Core/Core.h"
#include "Nest/Core/Input.h"

namespace Nest
{
	class WindowsInput : public Input
	{
	private:
		virtual bool isKeyPressedImpl(int keyCode);
	};
}