#pragma once

#include <maths/Common.h>
#include <maths/Mat4.h>
#include <geometry/Vector2.h>
#include <geometry/Vector4.h>

#include "Nest/Core/Core.h"
#include "Nest/renderer/Shader.h"
#include "Nest/renderer/Texture.h"

#include "Nest/graphics/Font.h"

namespace Nest
{
	namespace Renderer2D
	{
		void init();
		void shutdown();

		void beginScene(const chcl::Mat4 &mvp);
		void endScene();

		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color = 1.f);
		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, Ref<Shader> shader);
		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color = 1.f);
		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, Ref<Shader> shader);
		void drawLineQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color = 1.f, float weight = 1.f);
		void drawLineQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color = 1.f, float weight = 1.f);
		void drawCircle(chcl::Vector2<float> pos, chcl::Vector2<float> size, float thickness = 0, chcl::Vector4<float> fillColor = 1.f, chcl::Vector4<float> outlineColor = 1.f);

		void drawTexturedQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, Ref<Texture> texture);

		void drawText(chcl::Vector2<float> pos, float pnt, const std::string &text, Ref<Font> font);

		void drawFullScreenQuad();
		void drawFullScreenQuad(Ref<Shader> shader);
		void drawFullScreenQuad(chcl::Vector4<float> color = 1.f);

		void drawLine(chcl::Vector2<float> v1, chcl::Vector2<float> v2, chcl::Vector4<float> color = 1.f, float width = 1.f);
	}
}