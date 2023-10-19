#include "VertexBufferLayout.h"

#include <glad/glad.h>

namespace Nest
{
	unsigned int VertexBufferLayout::VBLayoutElement::GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	}

	template <>
	void VertexBufferLayout::push<float>(unsigned int count, unsigned int advanceCount)
	{
		m_layoutElements.push_back({ GL_FLOAT, count, advanceCount, GL_FALSE });
		m_stride += count * VBLayoutElement::GetSizeOfType(GL_FLOAT);
	}

	template <>
	void VertexBufferLayout::push<unsigned int>(unsigned int count, unsigned int advanceCount)
	{
		m_layoutElements.push_back({ GL_UNSIGNED_INT, count, advanceCount, GL_FALSE });
		m_stride += count * VBLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void VertexBufferLayout::push<unsigned char>(unsigned int count, unsigned int advanceCount)
	{
		m_layoutElements.push_back({ GL_UNSIGNED_BYTE, count, advanceCount, GL_TRUE });
		m_stride += count * VBLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
}