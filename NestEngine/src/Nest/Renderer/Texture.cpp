#include "Texture.h"

#include <glad/glad.h>

#include "Nest/Core/Core.h"

Nest::Texture::Texture()
{
	glGenTextures(1, &m_rendererID);
}

Nest::Texture::Texture(Texture &&tex) noexcept
{
	m_rendererID = tex.m_rendererID;
	m_width = tex.m_width;
	m_height = tex.m_height;
	tex.m_rendererID = 0;
}

Nest::Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererID);
}

void Nest::Texture::setData(size_t width, size_t height, DataType dataType, uint8_t pixelChannels, const void *data)
{
	m_width = width;
	m_height = height;

	bind(0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, (GLint)GetInternalFormat(dataType, pixelChannels), (GLuint)width, (GLuint)height, 0, (GLenum)GetFormat(pixelChannels), (GLenum)toGAPINativeType(dataType), data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Nest::Texture::setData(chcl::Vector2<size_t> size, DataType dataType, uint8_t pixelChannels, const void * data)
{
	setData(size.x, size.y, dataType, pixelChannels, data);
}

void Nest::Texture::bind(unsigned int textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Nest::Texture& Nest::Texture::operator=(Texture &&other) noexcept
{
	unsigned int tempID = m_rendererID;
	m_rendererID = other.m_rendererID;
	other.m_rendererID = tempID;
	return *this;
}

size_t Nest::Texture::GetFormat(uint8_t pixelChannels)
{
	switch (pixelChannels)
	{
		case 1:
			return GL_RED;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
	}

	NE_ASSERT(0, "Unknown data format.");
	return 0;
}

size_t Nest::Texture::GetInternalFormat(DataType dataType, uint8_t pixelChannels)
{
	switch (toGAPINativeType(dataType))
	{
		case GL_UNSIGNED_BYTE:
		case GL_BYTE:
		{
			switch (pixelChannels)
			{
				case 1:
					return GL_R8;
				case 2:
					return GL_RG8;
				case 3:
					return GL_RGB8;
				case 4:
					return GL_RGBA8;
			}
			break;
		}
		case GL_UNSIGNED_SHORT:
		case GL_SHORT:
		{
			switch (pixelChannels)
			{
				case 1:
					return GL_R16_SNORM;
				case 2:
					return GL_RG16_SNORM;
				case 3:
					return GL_RGB16_SNORM;
				case 4:
					return GL_RGBA16_SNORM;
			}
			break;
		}
		case GL_FLOAT:
		{
			switch (pixelChannels)
			{
				case 1:
					return GL_R16F;
				case 2:
					return GL_RG16F;
				case 3:
					return GL_RGB16F;
				case 4:
					return GL_RGBA16F;
			}
			break;
		}
	}

	switch (pixelChannels)
	{
		case 1:
			return GL_RED;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
	}

	NE_ASSERT(0, "Unknown data format.");
	return 0;
}