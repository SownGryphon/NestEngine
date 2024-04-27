#include "Texture.h"

#include <glad/glad.h>

#include "Nest/Core/Core.h"

template<>
Nest::Texture::Format Nest::Texture::Format::create<unsigned char>(unsigned int pixelDataCount)
{
	Format format;
	format.m_format = GetFormat(pixelDataCount);
	format.m_type = GL_UNSIGNED_BYTE;
	return format;
}

template<>
Nest::Texture::Format Nest::Texture::Format::create<int>(unsigned int pixelDataCount)
{
	Format format;
	format.m_format = GetFormat(pixelDataCount);
	format.m_type = GL_INT;
	return format;
}

template<>
Nest::Texture::Format Nest::Texture::Format::create<float>(unsigned int pixelDataCount)
{
	Format format;
	format.m_format = GetFormat(pixelDataCount);
	format.m_type = GL_FLOAT;
	return format;
}

unsigned int Nest::Texture::Format::GetFormat(unsigned int pixelDataCount)
{
	switch (pixelDataCount)
	{
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
	}

	NE_ASSERT(false, "Invalid pixel data count.")
	return -1;
}

Nest::Texture::Texture()
{
	glGenTextures(1, &m_rendererID);
}

Nest::Texture::Texture(Texture &&tex)
{
	m_rendererID = tex.m_rendererID;
	tex.m_rendererID = 0;
}

Nest::Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererID);
}

void Nest::Texture::setData(size_t width, size_t height, Format format, void *data)
{
	glTexImage2D(GL_TEXTURE_2D, 0, format.m_format, width, height, 0, format.m_format, format.m_type, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Nest::Texture::setData(chcl::Vector2<size_t> size, Format format, void * data)
{
	setData(size.x, size.y, format, data);
}

void Nest::Texture::bind(unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Nest::Texture& Nest::Texture::operator=(Texture &&other)
{
	unsigned int tempID = m_rendererID;
	m_rendererID = other.m_rendererID;
	other.m_rendererID = tempID;
	return *this;
}