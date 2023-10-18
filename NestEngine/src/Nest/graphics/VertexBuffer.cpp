#include "VertexBuffer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Nest
{
	VertexBuffer::VertexBuffer(unsigned int size, void *data)
	{
		glGenBuffers(1, &m_rendererID);
		bind();
		if (data)
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		else
			glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::setData(unsigned int size, void *data, unsigned int offset)
	{
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
}