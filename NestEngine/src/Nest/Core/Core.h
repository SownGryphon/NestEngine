#pragma once

#include <memory>

#include "Nest/Log/Logger.h"

#ifdef NE_PLATFORM_WINDOWS
#else
	#error Nest only supports Windows
#endif

#ifdef NE_ENABLE_ASSERTS
	#define NE_ASSERT(x, ...) { if (!(x)) { NE_WARN(__VA_ARGS__); __debugbreak(); }}
#else
	#define NE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << (x))

#define NE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Nest
{
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template <typename T, typename ...Args>
	constexpr Scope<T> createScope(Args &&...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename ...Args>
	constexpr Ref<T> createRef(Args &&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	
}