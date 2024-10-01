#pragma once

#include "Nest/Core/Core.h"

namespace Nest
{
	enum class DataType
	{
		None = 0,
		Bool,
		UnsignedByte, SignedByte,
		UnsignedShort, SignedShort,
		UnsignedInt, SignedInt,
		Float, Double
	};

	size_t toGAPINativeType(DataType type);

	template <typename T>
	DataType getDataType()
	{
		NE_ASSERT(0, "Unknown data type.");
		return DataType::None;
	}

	template <>	DataType getDataType<bool>();
	template <>	DataType getDataType<unsigned char>();
	template <>	DataType getDataType<char>();
	template <>	DataType getDataType<unsigned short>();
	template <>	DataType getDataType<short>();
	template <>	DataType getDataType<unsigned int>();
	template <>	DataType getDataType<int>();
	template <>	DataType getDataType<float>();
	template <>	DataType getDataType<double>();

	size_t getDataTypeSize(DataType type);

	template <typename T>
	size_t getGAPINativeType()
	{
		return toGAPINativeType(getDataType<T>());
	}
}