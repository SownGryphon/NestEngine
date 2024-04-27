#pragma once

#include "Nest/Core/Core.h"
#include "Nest/Core/Input.h"

namespace Nest
{
	class WindowsInput : public Input
	{
	private:
		virtual bool isKeyPressedImpl(int keyCode) override;

		virtual bool isMouseButtonDownImpl(int button) override;
		inline virtual float getMouseXImpl() override { return getMousePosImpl().x; }
		inline virtual float getMouseYImpl() override { return getMousePosImpl().y; }
		virtual chcl::Vector2<float> getMousePosImpl() override;
	};
}