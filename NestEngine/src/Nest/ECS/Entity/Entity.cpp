#include "Entity.h"

Nest::IDType Nest::ECS::Entity::s_globalID = 0;

Nest::ECS::Entity::Entity() :
	m_entityID(++s_globalID)
{}

Nest::ECS::Entity::Entity(Entity &&other) noexcept :
	m_entityID(other.m_entityID)
{
	other.m_entityID = 0;
}

Nest::ECS::Entity& Nest::ECS::Entity::operator=(Entity &&other) noexcept
{
	Nest::IDType temp = m_entityID;
	m_entityID = other.m_entityID;
	other.m_entityID = temp;

	return *this;
}