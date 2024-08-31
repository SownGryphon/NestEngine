#include "Buffer.h"

#include "geometry/VectorN.h"

Nest::Buffer::Buffer(size_t size, const uint8_t *data)
{
	reserve(size, false);

	if (data && m_data)
		memcpy(m_data, data, size);
}

Nest::Buffer::Buffer(const Buffer &other) :
	Buffer(other.m_bytesUsed)
{
	memcpy(m_data, other.m_data, other.m_bytesUsed);
	m_bytesUsed = other.m_bytesUsed;
}

Nest::Buffer::Buffer(Buffer &&other) noexcept :
	m_bytesUsed(other.m_bytesUsed), m_data(other.m_data)
{
	other.m_bytesUsed = 0;
	other.m_data = nullptr;
}

Nest::Buffer::~Buffer()
{
	if (m_data)
		operator delete(m_data, m_reserved);
}

Nest::Buffer& Nest::Buffer::operator=(const Buffer &other)
{
	if (m_reserved < other.m_bytesUsed)
		reserve(other.m_bytesUsed, false);

	memcpy(m_data, other.m_data, other.m_bytesUsed);
	m_bytesUsed = other.m_bytesUsed;
	return *this;
}

Nest::Buffer& Nest::Buffer::operator=(Buffer &&other) noexcept
{
	std::swap(m_bytesUsed, other.m_bytesUsed);
	std::swap(m_reserved, other.m_reserved);
	std::swap(m_data, other.m_data);
	return *this;
}

void Nest::Buffer::push_back(const uint8_t *src, size_t byteCount)
{
	if (m_bytesUsed + byteCount > m_reserved)
		reserve(3 * (m_reserved + byteCount) / 2, true);

	memcpy(m_data + m_bytesUsed, src, byteCount);
	m_bytesUsed += byteCount;
}

void Nest::Buffer::reserve(size_t newSize, bool retainData)
{
	if (newSize <= m_reserved)
		return;
	
	uint8_t* newData = (uint8_t*)operator new(newSize);

	if (m_data)
	{
		if (retainData)
			memcpy(newData, m_data, m_bytesUsed);
		else
			m_bytesUsed = 0;
		operator delete(m_data, m_reserved);
	}
	m_data = newData;
	m_reserved = newSize;
}

void Nest::Buffer::clear()
{
	if (!m_data)
		return;

	operator delete(m_data, m_reserved);
	m_bytesUsed = 0;
	m_reserved = 0;
	m_data = nullptr;
}