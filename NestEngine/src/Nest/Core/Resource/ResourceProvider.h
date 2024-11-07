#pragma once

#include "Nest/Core/Core.h"

namespace Nest
{
	template <typename T>
	class ResourceStorageBlock
	{
	private:
		T m_val;
		IDType m_updateCount = 0;

	public:
		ResourceStorageBlock() = default;
		ResourceStorageBlock(const T &val, IDType initialUpdateCount) :
			m_val(val), m_updateCount(initialUpdateCount)
		{}

		const T& peekValue() const { return m_val; }

		T& getValue()
		{
			++m_updateCount;
			return m_val;
		}

		void setValue(const T &value)
		{
			m_val = value;
			++m_updateCount;
		}

		IDType getUpdateCount() const { return m_updateCount; }
	};

	template <typename T>
	class ResourceProvider
	{
	private:
		Ref<ResourceStorageBlock<T>> m_resourceBlock;
		mutable IDType m_lastUpdateCount = 0;

	public:
		ResourceProvider()
		{
			m_resourceBlock = createRef<ResourceStorageBlock<T>>();
		}
		
		ResourceProvider(const T &val)
		{
			m_resourceBlock = createRef<ResourceStorageBlock<T>>(val, 1);
		}
		
		ResourceProvider(const ResourceProvider<T> &other)
		{
			m_resourceBlock = other.m_resourceBlock;
		}

		const T& peek() const
		{
			return m_resourceBlock->peekValue();
		}

		const T& get() const
		{
			m_lastUpdateCount = m_resourceBlock->getUpdateCount();
			return m_resourceBlock->peekValue();
		}

		T& get()
		{
			T &res = m_resourceBlock->getValue();
			m_lastUpdateCount = m_resourceBlock->getUpdateCount();
			return res;
		}

		void set(const T& value)
		{
			m_resourceBlock->setValue(value);
		}

		bool isUpToDate() const
		{
			return m_lastUpdateCount == m_resourceBlock->getUpdateCount();
		}

		ResourceProvider<T>& operator=(const ResourceProvider<T> &other)
		{
			m_resourceBlock = other.m_resourceBlock;
			m_lastUpdateCount = 0;
		}
	};
}