#include "VertexBufferLayout.h"

#include <glad/glad.h>

namespace Nest
{

	//template <>
	//void VertexBufferLayout::push<char>(unsigned int count, unsigned int advanceCount)
	//{
	//	m_layoutElements.push_back({ DataType::SignedByte, count, advanceCount, GL_TRUE });
	//	m_stride += count * getDataTypeSize(DataType::SignedByte);
	//}

	//template <>
	//void VertexBufferLayout::push<unsigned char>(unsigned int count, unsigned int advanceCount)
	//{
	//	m_layoutElements.push_back({ DataType::UnsignedByte, count, advanceCount, GL_TRUE });
	//	m_stride += count * getDataTypeSize(DataType::UnsignedByte);
	//}

	//template <>
	//void VertexBufferLayout::push<int>(unsigned int count, unsigned int advanceCount)
	//{
	//	m_layoutElements.push_back({ DataType::SignedInt, count, advanceCount, GL_FALSE });
	//	m_stride += count * getDataTypeSize(DataType::SignedInt);
	//}

	//template <>
	//void VertexBufferLayout::push<unsigned int>(unsigned int count, unsigned int advanceCount)
	//{
	//	m_layoutElements.push_back({ DataType::UnsignedInt, count, advanceCount, GL_FALSE });
	//	m_stride += count * getDataTypeSize(DataType::UnsignedInt);
	//}

	//template <>
	//void VertexBufferLayout::push<float>(unsigned int count, unsigned int advanceCount)
	//{
	//	m_layoutElements.push_back({ DataType::Float, count, advanceCount, GL_FALSE });
	//	m_stride += count * getDataTypeSize(DataType::Float);
	//}
}

bool Nest::VertexBufferLayout::IsNormalized(DataType type)
{
	switch (type)
	{
		case Nest::DataType::None:
			return false;
		case Nest::DataType::Bool:
			return true;
		case Nest::DataType::UnsignedByte:
		case Nest::DataType::SignedByte:
			return true;
		case Nest::DataType::UnsignedShort:
		case Nest::DataType::SignedShort:
			return false;
		case Nest::DataType::UnsignedInt:
		case Nest::DataType::SignedInt:
			return false;
		case Nest::DataType::Float:
			return false;
		case Nest::DataType::Double:
			return false;
	}

	NE_ASSERT(0, "Unknown data type");
	return false;
}