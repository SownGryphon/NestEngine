#include "ShaderStorageBuffer.h"

#include "platform/OpenGL/OpenGLCore.h"

Nest::ShaderStorageBuffer::ShaderStorageBuffer()
{
	glGenBuffers(1, &m_rendererID);
}

Nest::ShaderStorageBuffer::ShaderStorageBuffer(const char* data, size_t dataSize) :
	ShaderStorageBuffer()
{
	bind();
	glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, data, GL_STATIC_READ);
}

Nest::ShaderStorageBuffer::ShaderStorageBuffer(const Buffer &buf) :
	ShaderStorageBuffer()
{
	bind();
	glBufferData(GL_SHADER_STORAGE_BUFFER, buf.size(), buf.begin(), GL_STATIC_READ);
}

Nest::ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer &&other) noexcept
{
	m_rendererID = other.m_rendererID;
	other.m_rendererID = 0;
}

Nest::ShaderStorageBuffer::~ShaderStorageBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void Nest::ShaderStorageBuffer::bind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_rendererID);
}

void Nest::ShaderStorageBuffer::unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

Nest::ShaderStorageBuffer& Nest::ShaderStorageBuffer::operator=(ShaderStorageBuffer &&other) noexcept
{
	unsigned int temp = m_rendererID;
	m_rendererID = other.m_rendererID;
	other.m_rendererID = m_rendererID;

	return *this;
}