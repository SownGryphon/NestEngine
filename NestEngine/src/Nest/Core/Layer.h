#pragma once

#include <chrono>

#include "Nest/Events/Event.h"

namespace Nest
{
	class Layer
	{
	public:
		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(std::chrono::milliseconds timestep) {}
		virtual void onEvent(Event &e) {}
	};
}