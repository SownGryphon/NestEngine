#pragma once

#include "Nest/Core/Core.h"

#include "Nest/Renderer/GraphicsBuffer.h"

namespace Nest
{
	class IndexBuffer : public GraphicsBuffer<GL_ELEMENT_ARRAY_BUFFER>
	{
		size_t m_count = 0;

	public:
		IndexBuffer() : m_count(0) {}
		IndexBuffer(size_t count, unsigned int *indices);
		IndexBuffer(const IndexBuffer&) = delete;

		size_t getCount() const { return m_count; }
	};
}