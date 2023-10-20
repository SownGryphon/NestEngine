#pragma once

#include <vector>

#include "Layer.h"

namespace Nest
{
	class LayerStack
	{
		using layerVector = std::vector<Layer*>;
	public:
		~LayerStack();

		void pushLayer(Layer *layer);
		void popLayer(Layer *layer);

		inline layerVector::iterator begin() { return m_layers.begin(); }
		inline layerVector::iterator end() { return m_layers.end(); }
		inline layerVector::reverse_iterator rbegin() { return m_layers.rbegin(); }
		inline layerVector::reverse_iterator rend() { return m_layers.rend(); }

		inline layerVector::const_iterator begin() const { return m_layers.begin(); }
		inline layerVector::const_iterator end() const { return m_layers.end(); }
		inline layerVector::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		inline layerVector::const_reverse_iterator rend() const { return m_layers.rend(); }

	private:
		std::vector<Layer*> m_layers;
	};
}
