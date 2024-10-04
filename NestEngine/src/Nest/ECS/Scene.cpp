#include "Scene.h"

void Nest::ECS::Scene::update()
{
	for (System &s : m_systems)
	{
		s.update(m_entities);
	}
}

void Nest::ECS::Scene::addEntity(size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		addEntity();
	}
}