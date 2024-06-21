#pragma once

#include "Nest/Core/Core.h"

namespace Nest
{
	class IndexBuffer
	{
		size_t m_count = 0;
		unsigned int m_rendererID = 0;

	public:
		IndexBuffer() : m_count(0), m_rendererID(0) {}
		IndexBuffer(size_t count, unsigned int *indices);
		IndexBuffer(const IndexBuffer&) = delete;
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		size_t getCount() const { return m_count; }
	};
}