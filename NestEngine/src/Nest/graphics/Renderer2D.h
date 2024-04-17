#pragma once

#include <maths/Common.h>
#include <geometry/Vector2.h>
#include <geometry/Vector4.h>

#include "Nest/Core/Core.h"
#include "Nest/Graphics/Shader.h"
#include "Nest/Graphics/OrthographicCamera.h"

namespace Nest
{
	namespace Renderer2D
	{
		void init();
		void shutdown();

		void beginScene(const OrthographicCamera &camera);
		void endScene();

		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });
		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });
		void drawLineQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });
		void drawLineQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });
		void drawCircle(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });
		void drawCircle(chcl::Vector2<float> pos, chcl::Vector2<float> size, float thickness, chcl::Vector4<float> fillColor = 1.f, chcl::Vector4<float> outlineColor = 1.f);

		void drawFullScreenQuad();
		void drawFullScreenQuad(Ref<Shader> shader);
		void drawFullScreenQuad(chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });

		void drawLine(chcl::Vector2<float> v1, chcl::Vector2<float> v2, chcl::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f });
	}
}