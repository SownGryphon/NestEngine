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
		Texture(Texture &&tex) noexcept;
		~Texture();

		void setData(size_t width, size_t height, DataType dataType, uint8_t pixelChannels, const void *data);
		void setData(chcl::Vector2<size_t> size, DataType dataType, uint8_t pixelChannels, const void *data);

		template <typename T>
		void setData(unsigned int pixelChannels, chcl::DynamicMatrix<T> data)
		{
			setData(data.cols(), data.rows(), getDataType<T>(), pixelChannels, (void*)data.data());
		}

		inline size_t getWidth() const { return m_width; }
		inline size_t getHeight() const { return m_height; }
		inline chcl::Vector2<size_t> getSize() const { return { m_width, m_height }; }

		void bind(unsigned int textureUnit) const;

		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture &&tex) noexcept;

	private:
		unsigned int m_rendererID = 0;
		size_t m_width = 0, m_height = 0;

		static size_t GetFormat(uint8_t pixelChannels);
		static size_t GetInternalFormat(DataType dataType, uint8_t pixelChannels);
	};
}