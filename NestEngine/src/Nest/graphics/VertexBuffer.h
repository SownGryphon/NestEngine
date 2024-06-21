#pragma once

#include "Nest/Core/Core.h"

namespace Nest
{
	class VertexBuffer
	{
		unsigned int m_rendererID = 0;

	public:
		VertexBuffer(size_t size, const void *data = nullptr);
		VertexBuffer(const VertexBuffer&) = delete;
		~VertexBuffer();

		void bind() const;
		void unbind() const;

		void setData(size_t size, const void *data, size_t offset = 0);
	};
}