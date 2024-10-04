#include "VertexArray.h"

#include <glad/glad.h>

namespace Nest
{
	unsigned int VertexArray::s_currentBuffer = 0;

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
		if (s_currentBuffer != m_rendererID)
		{
			s_currentBuffer = m_rendererID;
			glBindVertexArray(m_rendererID);
		}
	}

	void VertexArray::unbind() const
	{
		if (s_currentBuffer != 0)
		{
			s_currentBuffer = 0;
			glBindVertexArray(0);
		}
	}

	void VertexArray::bindBuffers(const Ref<VertexBuffer> &vBuffer, const Ref<VertexBufferLayout> &vbLayout)
	{
		bind();
		vBuffer->bind();
		size_t offset = 0;
		const auto& layout = vbLayout->getLayout();
		for (size_t i = 0; i < layout.size(); ++i)
		{
			glEnableVertexAttribArray((GLuint)i);

			switch (layout[i].shaderUsage)
			{
				case DataType::Bool:
				case DataType::UnsignedByte:
				case DataType::SignedByte:
				case DataType::UnsignedShort:
				case DataType::SignedShort:
				case DataType::UnsignedInt:
				case DataType::SignedInt:
					glVertexAttribIPointer((GLuint)i, (GLuint)layout[i].count, (GLenum)toGAPINativeType(layout[i].type), (GLuint)vbLayout->getStride(), (void*)offset);
					break;
				case DataType::Float:
				case DataType::Double:
					glVertexAttribPointer((GLuint)i, (GLuint)layout[i].count, (GLenum)toGAPINativeType(layout[i].type), layout[i].normalized ? GL_TRUE : GL_FALSE, (GLuint)vbLayout->getStride(), (void*)offset);
					break;
			}

			glVertexAttribDivisor((GLuint)i, layout[i].advanceCount);
			
			offset += layout[i].count * getDataTypeSize(layout[i].type);
		}

		m_vBuffer = vBuffer;
	}
}