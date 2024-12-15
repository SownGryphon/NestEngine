#include "Font.h"

#include <cstdint>
#include <fstream>

#include "Nest/renderer/Renderer.h"

Nest::Ref<Nest::VertexBufferLayout> Nest::Font::s_VBLayout;
Nest::Ref<Nest::Shader> Nest::Font::s_fontShader;

Nest::Font::Font()
{
	m_contourPoints = createRef<ShaderStorageBuffer>();
}

void Nest::Font::renderText(const std::string &text, chcl::Vector2<float> pos)
{
	if (!s_fontShader.get())
	{
		s_fontShader = Shader::FromFile("res\\shaders\\FontTest.glsl");
	}

	Ref<VertexArray> vao = generateVertexArray(text);

	m_contourPoints->bind();
	s_fontShader->bindBuffer(m_contourPoints, 1);

	Renderer::drawPoints(s_fontShader, vao, (unsigned int) text.length());
}

Nest::Ref<Nest::VertexArray> Nest::Font::generateVertexArray(const std::string &text)
{
	Buffer vbData;

	float xOff = 0.f, yOff = 0.f;

	for (char c : text)
	{
		SimpleGlyph glyph = m_glyphs[m_charMapping[c]];

		for (uint8_t j = 0; j < 4; ++j)
		{
			bool xMin = ~j & BIT(0);
			bool yMin = ~j & BIT(1);

			// Vertex window x
			vbData.push_back<float>(xOff - glyph.leftSideBearing + (xMin ? glyph.xMin : glyph.xMax));
			// Vertex window y
			vbData.push_back<float>((yMin ? glyph.yMin : glyph.yMax) + yOff);
			// Vertex fUnit x
			vbData.push_back<float>(xMin ? glyph.xMin : glyph.xMax);
			// Vertex fUnit y
			vbData.push_back<float>(yMin ? glyph.yMin : glyph.yMax);

			vbData.push_back<uint32_t>(glyph.contourBegin);
			vbData.push_back<uint32_t>(glyph.numContourPoints);
		}

		if (c == '\n')
		{
			yOff -= (float)m_uPerEm;
			xOff = 0.f;
		}
		else
		{
			xOff += (float)glyph.advanceWidth;
		}
	}

	Ref<VertexBuffer> vb = createRef<VertexBuffer>(vbData);
	Ref<VertexArray> result = createRef<VertexArray>();
	result->bindBuffers(vb, getLayout());
	return result;
}

Nest::Ref<Nest::IndexBuffer> Nest::Font::generateIndexBuffer(const std::string &text)
{
	std::vector<unsigned int> indices;
	indices.reserve(text.length() * 6);
	for (size_t i = 0; i < text.length(); ++i)
	{
		indices.push_back((unsigned int)i * 4);
		indices.push_back((unsigned int)i * 4 + 1);
		indices.push_back((unsigned int)i * 4 + 2);
		indices.push_back((unsigned int)i * 4 + 2);
		indices.push_back((unsigned int)i * 4 + 1);
		indices.push_back((unsigned int)i * 4 + 3);
	}

	//indices.push_back(1);
	//indices.push_back(2);
	//indices.push_back(3);

	return createRef<IndexBuffer>(indices.size(), indices.data());
}

Nest::Ref<Nest::VertexBufferLayout> Nest::Font::getLayout()
{
	if (!s_VBLayout.get())
	{
		s_VBLayout = createRef<VertexBufferLayout>();
		s_VBLayout->push<float>(2);		// Vertex (x,y)
		s_VBLayout->push<float>(2);		// Vertex glyph (x,y) in f-units
		s_VBLayout->push<uint32_t>(1);	// Offset to glyph beginning
		s_VBLayout->push<uint32_t>(1);	// Number of points in contour
		//s_VBLayout->push<int32_t>(1, DataType::None, true, 4);
		//s_VBLayout->push<int32_t>(1, DataType::None, true, 4);
	}

	return s_VBLayout;
}