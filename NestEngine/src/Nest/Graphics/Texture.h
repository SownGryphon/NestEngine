#pragma once

#include <geometry/Vector2.h>
#include <maths/DynamicMatrix.h>

namespace Nest
{
	class Texture
	{
	public:
		struct Format
		{
			Format() {};

			template <typename T>
			static Format create(unsigned int pixelDataCount)
			{
				std::runtime_error(false);
			}

			template<> static Format create<unsigned char>(unsigned int pixelDataCount);
			template<> static Format create<int>(unsigned int pixelDataCount);
			template<> static Format create<float>(unsigned int pixelDataCount);

			unsigned int m_format = 0, m_type = 0;

		private:
			static unsigned int GetFormat(unsigned int pixelDataCount);
		};

		Texture();
		Texture(const Texture&) = delete;
		Texture(Texture &&tex);
		~Texture();

		void setData(size_t width, size_t height, Format format, void *data);
		void setData(chcl::Vector2<size_t> size, Format format, void *data);
		template <typename T>
		void setData(unsigned int pixelDataCount, chcl::DynamicMatrix<T> data)
		{
			setData(data.cols(), data.rows(), Format::create<T>(pixelDataCount), (void*)data.data());
		}

		void bind(unsigned int textureUnit);

		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture &&tex);

	private:
		unsigned int m_rendererID;
	};
}