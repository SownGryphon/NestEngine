#pragma once

#include <type_traits>

#include "Nest/Core/Resource/ResourceProvider.h"

namespace Nest
{
	template <typename T>
	class StaticResourceProvider : public ResourceProvider<T>
	{
	protected:
		T m_value;

	public:
		StaticResourceProvider() : m_value() {}
		StaticResourceProvider(const T &val) : m_value(val) {}

		virtual const T& get() const override { return m_value; }
		virtual void set(const T &val) override
		{
			m_value = val;
		}
	};
}