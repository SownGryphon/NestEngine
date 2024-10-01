#include "VertexBuffer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

Nest::VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &m_rendererID);
}

Nest::VertexBuffer::VertexBuffer(size_t size, const void *data) :
	VertexBuffer()
{
	bind();
	if (data)
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
}

Nest::VertexBuffer::VertexBuffer(Buffer &buf) :
	VertexBuffer(buf.size(), buf.begin())
{}

Nest::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void Nest::VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void Nest::VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Nest::VertexBuffer::setData(size_t size, const void *data, size_t offset)
{
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
