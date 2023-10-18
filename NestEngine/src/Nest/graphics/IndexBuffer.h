#pragma once

#include "../Core.h"

namespace Nest
{
	class IndexBuffer
	{
		unsigned int m_count = 0;
		unsigned int m_rendererID = 0;

	public:
		IndexBuffer() : m_count(0), m_rendererID(0) {}
		IndexBuffer(unsigned int count, unsigned int *indices);
		IndexBuffer(const IndexBuffer&) = delete;
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		unsigned int getCount() const { return m_count; }
	};
}