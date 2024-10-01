#include "DataType.h"

#include "platform/OpenGL/OpenGLCore.h"

size_t Nest::toGAPINativeType(DataType type)
{
	return toOpenGLType(type);
}

template <>
Nest::DataType Nest::getDataType<bool>()
{
	return DataType::Bool;
}

template <>
Nest::DataType Nest::getDataType<unsigned char>()
{
	return DataType::UnsignedByte;
}

template <>
Nest::DataType Nest::getDataType<char>()
{
	return DataType::SignedByte;
}

template <>
Nest::DataType Nest::getDataType<unsigned short>()
{
	return DataType::UnsignedShort;
}

template <>
Nest::DataType Nest::getDataType<short>()
{
	return DataType::SignedShort;
}

template <>
Nest::DataType Nest::getDataType<unsigned int>()
{
	return DataType::UnsignedInt;
}

template <>
Nest::DataType Nest::getDataType<int>()
{
	return DataType::SignedInt;
}

template <>
Nest::DataType Nest::getDataType<float>()
{
	return DataType::Float;
}

template <>
Nest::DataType Nest::getDataType<double>()
{
	return DataType::Double;
}

size_t Nest::getDataTypeSize(DataType type)
{
	switch (type)
	{
		case Nest::DataType::None:
			return 0;
		case DataType::Bool:
			return 1;
		case DataType::UnsignedByte:
		case DataType::SignedByte:
			return 1;
		case DataType::UnsignedShort:
		case DataType::SignedShort:
			return 2;
		case DataType::UnsignedInt:
		case DataType::SignedInt:
			return 4;
		case DataType::Float:
			return 4;
		case DataType::Double:
			return 8;
	}

	NE_ASSERT(0, "Invalid data type");
	return 0;
}