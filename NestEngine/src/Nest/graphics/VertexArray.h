#pragma once

#include "Nest/Core/Core.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Nest
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		~VertexArray();

		void bind() const;
		void unbind() const;

		void bindBuffers(const Ref<VertexBuffer> &vBuffer, const Ref<VertexBufferLayout> &vbLayout);

		inline Ref<VertexBuffer> getVertexBuffer() { return m_vBuffer; }

	private:
		unsigned int m_rendererID = 0;

		Ref<VertexBuffer> m_vBuffer;
	};
}