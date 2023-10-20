#pragma once

#include "Nest/Events/Event.h"

namespace Nest
{
	class Layer
	{
	public:
		virtual void onAttach() = 0;
		virtual void onDetach() = 0;
		virtual void onUpdate() = 0;
		virtual void onEvent(Event &e) = 0;
	};
}