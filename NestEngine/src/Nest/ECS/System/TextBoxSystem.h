#pragma once

#include "Nest/Core/Core.h"

#include "Nest/ECS/System/System.h"
#include "Nest/ECS/Component/TextBoxComponent.h"

#include "Nest/Renderer/Renderer2D.h"

namespace Nest::ECS::TextBoxSystem
{
	inline System Create()
	{
		return System::Create<TextBoxComp>(
			[](Entity &e)
			{
				Renderer2D::drawTextBox(TextBoxComp::Get().getData(e));
			}
		);
	}
}