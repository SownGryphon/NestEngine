#pragma once

#include <glad/glad.h>

#include "Nest/Core/Buffer.h"

namespace Nest
{
	template <unsigned int targetBuffer>
	class GraphicsBuffer
	{
	private:
		static unsigned int s_currentBoundBuffer;

	protected:
		unsigned int m_rendererID = 0;

	public:
		GraphicsBuffer()
		{
			glGenBuffers(1, &m_rendererID);
		}

		GraphicsBuffer(size_t size, const void *data = nullptr) :
			GraphicsBuffer()
		{
			bind();
			if (data)
				glBufferData(targetBuffer, size, data, GL_STATIC_DRAW);
			else
				glBufferData(targetBuffer, size, NULL, GL_DYNAMIC_DRAW);
		}

		GraphicsBuffer(Buffer &buf) :
			GraphicsBuffer(buf.size(), buf.begin())
		{}

		~GraphicsBuffer()
		{
			unbind();
			glDeleteBuffers(1, &m_rendererID);
		}

		GraphicsBuffer& operator=(const GraphicsBuffer&) = delete;
		GraphicsBuffer& operator=(GraphicsBuffer&& other) noexcept
		{
			unsigned int temp = m_rendererID;
			m_rendererID = other.m_rendererID;
			other.m_rendererID = m_rendererID;

			return *this;
		}

		void bind() const
		{
			if (s_currentBoundBuffer != m_rendererID)
			{
				s_currentBoundBuffer = m_rendererID;
				glBindBuffer(targetBuffer, m_rendererID);
			}
		}

		void unbind() const
		{
			if (s_currentBoundBuffer != 0)
			{
				s_currentBoundBuffer = 0;
				glBindBuffer(targetBuffer, 0);
			}
		}

		void setData(size_t size, const void *data, size_t offset = 0)
		{
			bind();
			glBufferSubData(targetBuffer, offset, size, data);
		}

		inline unsigned int getID() const { return m_rendererID; }
	};

	template <unsigned int i>
	unsigned int GraphicsBuffer<i>::s_currentBoundBuffer;
}