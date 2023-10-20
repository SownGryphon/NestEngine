#pragma once

#include <stdexcept>
#include <vector>

#include "Nest/Core/Core.h"

namespace Nest
{
	class VertexBufferLayout
	{
	public:
		struct VBLayoutElement
		{
			unsigned int type;
			unsigned int count;
			unsigned int advanceCount;
			bool normalized;

			static unsigned int GetSizeOfType(unsigned int type);
		};

	private:
		unsigned int m_stride = 0;
		std::vector<VBLayoutElement> m_layoutElements;

	public:
		inline const unsigned int getStride() const { return m_stride; }
		inline const std::vector<VBLayoutElement>& getLayout() const { return m_layoutElements; }

		template <typename T>
		void push(unsigned int count, unsigned int  = 0)
		{
			std::runtime_error(false);
		}

		template <> void push<float>(unsigned int count, unsigned int advanceCount);
		template <> void push<unsigned int>(unsigned int count, unsigned int advanceCount);
		template <> void push<unsigned char>(unsigned int count, unsigned int advanceCount);
	};
}