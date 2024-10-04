#include "IndexBuffer.h"

#include <glad/glad.h>

namespace Nest
{
	IndexBuffer::IndexBuffer(size_t count, unsigned int *indices) :
		GraphicsBuffer(count * sizeof(unsigned int), indices),
		m_count(count)
	{}
}