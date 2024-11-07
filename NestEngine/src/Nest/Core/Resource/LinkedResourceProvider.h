#pragma once

#include "Nest/Core/Core.h"

#include "Nest/Core/Resource/ResourceProvider.h"

namespace Nest
{
	template <typename T>
	class LinkedResourceProvider : public ResourceProvider<T>
	{
	private:
		using SourceType = ResourceProvider<T>;

	protected:
		Ref<SourceType> m_source;

	public:
		LinkedResourceProvider(Ref<SourceType> source) :
			m_source()
		{}

		virtual const T& get() const override
		{
			return m_source->get();
		}

		virtual void set(const T &val) override
		{
			m_source->set(val);
		}
	};
}