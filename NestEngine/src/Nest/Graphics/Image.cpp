#include "Image.h"

#include <geometry/Vector3.h>

Nest::Image::Image() :
	m_width(2), m_height(2), m_dataType(DataType::UnsignedByte), m_channels(3), m_imageData(12)
{
	m_imageData.push_back(chcl::Vector3<uint8_t>(0, 0, 0));
	m_imageData.push_back(chcl::Vector3<uint8_t>(0xff, 0, 0xff));
	m_imageData.push_back(chcl::Vector3<uint8_t>(0xff, 0, 0xff));
	m_imageData.push_back(chcl::Vector3<uint8_t>(0, 0, 0));
}

Nest::Image::Image(size_t imageWidth, size_t imageHeight, uint8_t imageChannels, DataType dataType, const uint8_t *data, ScanlineDirection dir) :
	m_width(imageWidth), m_height(imageHeight), m_channels(imageChannels), m_dataType(dataType), m_imageData(imageWidth * imageHeight * imageChannels * getDataTypeSize(dataType))
{
	// If there is no data to initialize the image data, fill it with zeroes
	if (!data)
	{
		m_imageData.fill<uint8_t>(0);
		return;
	}

	if (dir == ScanlineDirection::BottomToTop)
		m_imageData.push_back(data, m_imageData.reserved());
	else
	{
		size_t scanlineLen = m_width * m_channels * getDataTypeSize(m_dataType);
		for (size_t i = 0; i < m_height; ++i)
		{
			m_imageData.push_back(data + (m_height - i - 1) * scanlineLen, scanlineLen);
		}
	}
}

Nest::Image::Image(size_t imageWidth, size_t imageHeight, uint8_t imageChannels, DataType dataType, const Buffer &imageData, ScanlineDirection dir) :
	Image(imageWidth, imageHeight, imageChannels, dataType, imageData.begin(), dir)
{}

void Nest::Image::setPixel(size_t pixelX, size_t pixelY, const uint8_t *pixelData, ScanlineDirection dir)
{
	size_t scanlineLen = m_width * m_channels * getDataTypeSize(m_dataType);
	size_t scanline = dir == ScanlineDirection::BottomToTop ? pixelY : m_height - pixelY - 1;
	uint8_t pixelWidth = m_channels * (uint8_t)getDataTypeSize(m_dataType);

	memcpy(m_imageData.begin() + scanline * scanlineLen + pixelX * pixelWidth, pixelData, pixelWidth);
}

void Nest::Image::setPixelComponent(size_t pixelX, size_t pixelY, uint8_t component, const uint8_t *componentData, ScanlineDirection dir)
{
	size_t scanlineLen = m_width * m_channels * getDataTypeSize(m_dataType);
	size_t scanline = dir == ScanlineDirection::BottomToTop ? pixelY : m_height - pixelY - 1;
	uint8_t pixelWidth = m_channels * (uint8_t)getDataTypeSize(m_dataType);

	memcpy(m_imageData.begin() + scanline * scanlineLen + pixelX * pixelWidth + component * getDataTypeSize(m_dataType), componentData, getDataTypeSize(m_dataType));
}

void Nest::Image::setPixelScanline(size_t scanline, const uint8_t *scanlineData, ScanlineDirection dir)
{
	size_t scanlineLen = m_width * m_channels * getDataTypeSize(m_dataType);
	size_t scanlineActual = dir == ScanlineDirection::BottomToTop ? scanline : m_height - scanline - 1;

	memcpy(m_imageData.begin() + scanline * scanlineLen, scanlineData, scanlineLen);
}

Nest::Ref<Nest::Texture> Nest::Image::toTexture() const
{
	Ref<Texture> result = createRef<Texture>();
	result->setData(m_width, m_height, m_dataType, m_channels, m_imageData.begin());
	return result;
}