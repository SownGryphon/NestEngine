#include "OpenGLCore.h"

#include <glad/glad.h>

#include "Nest/Core/Core.h"

size_t Nest::toOpenGLType(DataType type)
{
	switch (type)
	{
		case DataType::None:
			return GL_NONE;
		case DataType::UnsignedByte:
			return GL_UNSIGNED_BYTE;
		case DataType::SignedByte:
			return GL_BYTE;
		case DataType::UnsignedShort:
			return GL_UNSIGNED_SHORT;
		case DataType::SignedShort:
			return GL_SHORT;
		case DataType::UnsignedInt:
			return GL_UNSIGNED_INT;
		case DataType::SignedInt:
			return GL_INT;
		case DataType::Float:
			return GL_FLOAT;
	}
	NE_ASSERT(0, "Invalid data type.");
	return 0;
}