#pragma once

#include <string>

#include <geometry/Vector2.h>

#include "Nest/Core/Core.h"

#include "Nest/Renderer/VertexArray.h"
#include "Nest/Renderer/IndexBuffer.h"

#include "Nest/Graphics/Font.h"
#include "Nest/Graphics/GraphicsElement.h"

namespace Nest
{
	class TextBox : public GraphicsElement
	{
	private:
		Ref<Font> m_font;
		Ref<VertexArray> m_textVA;
		Ref<IndexBuffer> m_textIB;

		chcl::Vector2<float> m_pos;
		float m_fontPoint;

		chcl::Mat4 m_transform;

	public:
		TextBox();
		TextBox(chcl::Vector2<float> pos, const std::string &text, float pnt, Ref<Font> font);

		void setPosition(chcl::Vector2<float> pos);
		void setText(const std::string &text);
		void setPnt(float pnt);

		//inline Ref<Shader> getShader() const override { return Font::s}
		inline Ref<Font> getFont() const { return m_font; }
		inline Ref<VertexArray> getVA() const override { return m_textVA; }
		inline Ref<IndexBuffer> getIB() const override { return m_textIB; }
		inline chcl::Mat4 getTransform() const override { return m_transform; }

	private:
		void updateTransform();
	};
}