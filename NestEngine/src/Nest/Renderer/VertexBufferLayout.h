#pragma once

#include <stdexcept>
#include <vector>

#include "Nest/Core/Core.h"
#include "Nest/Core/DataType.h"

namespace Nest
{
	class VertexBufferLayout
	{
	public:
		struct VBLayoutElement
		{
			DataType type, shaderUsage;
			size_t count;
			unsigned int advanceCount;
			bool normalized;
		};

	private:
		size_t m_stride = 0;
		std::vector<VBLayoutElement> m_layoutElements;

	public:
		inline const size_t getStride() const { return m_stride; }
		inline const std::vector<VBLayoutElement>& getLayout() const { return m_layoutElements; }

		template <typename T>
		void push(unsigned int count, DataType shaderUsage = DataType::None, bool normalized = true, unsigned int advanceCount = 0)
		{
			DataType type = getDataType<T>();
			m_layoutElements.push_back({ type, shaderUsage == DataType::None ? type : shaderUsage, count, advanceCount, normalized });
			m_stride += count * getDataTypeSize(type);
		}

		//template <> void push<bool>(unsigned int count, unsigned int advanceCount);
		//template <> void push<unsigned char>(unsigned int count, unsigned int advanceCount);
		//template <> void push<char>(unsigned int count, unsigned int advanceCount);
		//template <> void push<unsigned int>(unsigned int count, unsigned int advanceCount);
		//template <> void push<int>(unsigned int count, unsigned int advanceCount);
		//template <> void push<float>(unsigned int count, unsigned int advanceCount);

	private:

		static bool IsNormalized(DataType type);
	};
}