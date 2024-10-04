#pragma once

#include "Nest/ECS/System/System.h"

#include "Nest/ECS/Component/SpriteComponent.h"

#include "Nest/Renderer/Renderer2D.h"

namespace Nest::ECS::SpriteSystem
{
	inline System Create()
	{
		return System::Create<SpriteComponent>(
			[](Entity &e)
			{
				Sprite &sprite = SpriteComponent::Get().getData(e);
				Renderer2D::drawTexturedQuad(sprite.pos, (chcl::Vector2<float>)sprite.texture->getSize() * sprite.scale, sprite.texture);
			}
		);
	}
}