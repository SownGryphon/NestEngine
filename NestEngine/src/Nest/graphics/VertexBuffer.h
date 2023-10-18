#pragma once

#include "../Core.h"

namespace Nest
{
	class VertexBuffer
	{
		unsigned int m_rendererID = 0;

	public:
		VertexBuffer(unsigned int size, void *data = nullptr);
		VertexBuffer(const VertexBuffer&) = delete;
		~VertexBuffer();

		void bind() const;
		void unbind() const;

		void setData(unsigned int size, void *data, unsigned int offset = 0);
	};
}