#pragma once

#include "Nest/Core/Buffer.h"

#include "Nest/renderer/Texture.h"

namespace Nest
{
	enum class ScanlineDirection
	{
		TopToBottom, BottomToTop
	};

	class Image
	{
	public:
		Image();

		Image(size_t imageWidth, size_t imageHeight, uint8_t imageChannels, DataType dataType, const uint8_t *data = nullptr, ScanlineDirection dir = ScanlineDirection::TopToBottom);
		Image(size_t imageWidth, size_t imageHeight, uint8_t imageChannels, DataType dataType, const Buffer &imageData, ScanlineDirection dir = ScanlineDirection::TopToBottom);

		void setPixel(size_t pixelX, size_t pixelY, const uint8_t *pixelData, ScanlineDirection dir = ScanlineDirection::TopToBottom);
		void setPixelComponent(size_t pixelX, size_t pixelY, uint8_t component, const uint8_t *componentData, ScanlineDirection dir = ScanlineDirection::TopToBottom);
		void setPixelScanline(size_t scanline, const uint8_t *scanlineData, ScanlineDirection dir = ScanlineDirection::TopToBottom);

		Ref<Texture> toTexture() const;

	private:
		size_t m_width, m_height;
		uint8_t m_channels;
		DataType m_dataType;
		Buffer m_imageData;
	};
}