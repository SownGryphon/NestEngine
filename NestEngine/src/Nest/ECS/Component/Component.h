#pragma once

#include <unordered_map>
#include <vector>

#include "Nest/Core/Core.h"

#include "Nest/ECS/Entity/Entity.h"

#include "geometry/Vector2.h"
#include "geometry/Vector3.h"

namespace Nest
{
	namespace ECS
	{
		template <typename T>
		class Component
		{
		protected:
			using ComponentDataType = T;

			std::unordered_map<Nest::IDType, ComponentDataType> m_entityData;

			Component() = default;

		public:
			Component(const Component&) = delete;

			static Component& Get()
			{
				static Component s_instance;
				return s_instance;
			}

			template <typename ...Args>
			void attachToEntity(Entity &entity, Args... args)
			{
				if (isAttachedToEntity(entity))
					return;

				m_entityData[entity.getEntityID()] = ComponentDataType(args...);
			}

			bool isAttachedToEntity(Entity &entity) const { return m_entityData.contains(entity.getEntityID()); }

			ComponentDataType& getData(Entity &entity) { return m_entityData.at(entity.getEntityID()); }
			const ComponentDataType& getData(Entity &entity) const { return m_entityData.at(entity.getEntityID()); }

			Component& operator=(const Component&) = delete;
		};

		using Position2DComp = Component<chcl::Vector2<float>>;
		using Position3DComp = Component<chcl::Vector3<float>>;
	}
}