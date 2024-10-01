#pragma once

#include <geometry/Vector2.h>
#include <maths/DynamicMatrix.h>

#include "Nest/Core/DataType.h"

namespace Nest
{
	class Texture
	{
	public:
		Texture();
		Texture(const Texture&) = delete;
		Texture(Texture &&tex);
		~Texture();

		void setData(size_t width, size_t height, DataType dataType, uint8_t pixelChannels, const void *data);
		void setData(chcl::Vector2<size_t> size, DataType dataType, uint8_t pixelChannels, const void *data);
		template <typename T>
		void setData(unsigned int pixelChannels, chcl::DynamicMatrix<T> data)
		{
			setData(data.cols(), data.rows(), getDataType<T>(), pixelChannels, (void*)data.data());
		}

		void bind(unsigned int textureUnit);

		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture &&tex);

	private:
		unsigned int m_rendererID;

		static size_t GetFormat(uint8_t pixelChannels);
		static size_t GetInternalFormat(DataType dataType, uint8_t pixelChannels);
	};
}