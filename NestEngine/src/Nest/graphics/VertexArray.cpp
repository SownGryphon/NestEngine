#include "VertexArray.h"

#include <glad/glad.h>

namespace Nest
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_rendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_rendererID);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::bindBuffers(const Ref<VertexBuffer> &vBuffer, const Ref<VertexBufferLayout> &vbLayout)
	{
		bind();
		vBuffer->bind();
		unsigned int offset = 0;
		const auto& layout = vbLayout->getLayout();
		for (unsigned int i = 0; i < layout.size(); ++i)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribDivisor(i, layout[i].advanceCount);
			glVertexAttribPointer(i, layout[i].count, layout[i].type, layout[i].normalized, vbLayout->getStride(), (void*)offset);
			offset += layout[i].count * VertexBufferLayout::VBLayoutElement::GetSizeOfType(layout[i].type);
		}

		m_vBuffer = vBuffer;
	}
}