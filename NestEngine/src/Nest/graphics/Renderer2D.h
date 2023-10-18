#pragma once

#include "../Core.h"

#include <maths/Common.h>
#include <geometry/Vector2.h>
#include <geometry/Vector4.h>

namespace Nest
{
	namespace Renderer2D
	{
		void init();
		void shutdown();

		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });
		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });
	}
}