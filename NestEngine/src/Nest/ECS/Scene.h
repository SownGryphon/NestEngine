#pragma once

#include <vector>

#include "Nest/ECS/Entity/Entity.h"

#include "Nest/ECS/System/System.h"

namespace Nest::ECS
{
	class Scene
	{
	public:
		std::vector<Entity> m_entities;
		
	private:
		std::vector<System> m_systems;

	public:
		Scene() = default;

		inline void addEntity() { m_entities.emplace_back(); }
		void addEntity(size_t count);
		inline void addEntity(Entity &&e) { m_entities.emplace_back(std::move(e)); }

		inline void addSystem(System &s) { m_systems.emplace_back(s); }
		inline void addSystem(System &&s) { m_systems.emplace_back(std::move(s)); }

		void update();
	};
}