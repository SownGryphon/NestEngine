#pragma once

#include <functional>
#include <vector>

#include "Nest/Core/Core.h"

#include "Nest/ECS/Entity/Entity.h"
#include "Nest/ECS/Component/Component.h"

namespace Nest
{
	namespace ECS
	{
		template <typename>
		struct is_component : public std::false_type {};

		template <typename T>
		struct is_component<Component<T>> : public std::true_type {};

		template <typename T>
		concept ComponentType = is_component<T>::value;

		class System
		{
		private:
			std::function<bool(Entity&)> m_inSystemFunc;
			std::function<void(Entity&)> m_updateFunc;

			System(std::function<bool(Entity&)> inSystemFunc, std::function<void(Entity&)> updateFunc) :
				m_inSystemFunc(inSystemFunc),
				m_updateFunc(updateFunc)
			{}

		public:
			template <ComponentType ...Comps>
			static System Create(std::function<void(Entity&)> updateFunc)
			{
				return System(
					std::bind(&System::IsEntityInSystem<void, Comps...>, std::placeholders::_1),
					updateFunc
				);
			}

			void update(std::vector<Entity> &entities)
			{
				for (Entity &e : entities)
				{
					if (m_inSystemFunc(e))
					{
						m_updateFunc(e);
					}
				}
			}

		private:
			template <typename, ComponentType T, ComponentType ...Args>
			static bool IsEntityInSystem(Entity &e)
			{
				if (!T::Get().isAttachedToEntity(e))
					return false;

				return IsEntityInSystem<void, Args...>(e);
			}

			template <typename>
			static bool IsEntityInSystem(Entity &e)
			{
				return true;
			}
		};
	}
}