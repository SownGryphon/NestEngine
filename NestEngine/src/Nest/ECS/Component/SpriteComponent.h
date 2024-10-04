#pragma once

#include <geometry/Vector2.h>

#include "Nest/Core/Core.h"

#include "Nest/ECS/Component/Component.h"

#include "Nest/Renderer/Texture.h"

namespace Nest::ECS
{
	struct Sprite
	{
		Ref<Texture> texture;

		chcl::Vector2<float> pos;
		float scale;
		float rotation;
	};

	using SpriteComponent = ECS::Component<Sprite>;
}