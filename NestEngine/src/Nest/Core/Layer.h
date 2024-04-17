#pragma once

#include <chrono>

#include "Nest/Events/Event.h"

namespace Nest
{
	class Layer
	{
	public:
		virtual void onAttach() = 0;
		virtual void onDetach() = 0;
		virtual void onUpdate(std::chrono::milliseconds timestep) = 0;
		virtual void onEvent(Event &e) = 0;
	};
}