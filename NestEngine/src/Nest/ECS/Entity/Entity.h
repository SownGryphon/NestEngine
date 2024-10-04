#pragma once

#include <set>

#include "Nest/Core/Core.h"

namespace Nest
{
	namespace ECS
	{
		class Entity
		{
		private:
			static Nest::IDType s_globalID;
			
		protected:
			Nest::IDType m_entityID;

		public:
			Entity();
			Entity(const Entity&) = delete;
			Entity(Entity &&other) noexcept;

			inline Nest::IDType getEntityID() const { return m_entityID; }

			Entity& operator=(const Entity&) = delete;
			Entity& operator=(Entity &&other) noexcept;
		};
	}
}