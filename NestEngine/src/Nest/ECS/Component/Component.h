#pragma once

#include <stdexcept>
#include <vector>
#include <string>

#include "Nest/Core/Core.h"

#include "Nest/ECS/Entity/Entity.h"

#include "geometry/Vector2.h"
#include "geometry/Vector3.h"

namespace Nest
{
	namespace ECS
	{
		template <typename T>
		class ComponentDataList
		{
		private:
			struct DataListSpan
			{
				size_t listIndex;
				IDType firstEntity, lastEntity;

				size_t entityListIndex(IDType entityID) const
				{
					return listIndex + entityID - firstEntity;
				}

				size_t range() const
				{
					return size_t(lastEntity) - size_t(firstEntity);
				}
			};

			std::vector<DataListSpan> m_spans;
			std::vector<T> m_data;

		public:

			static const size_t npos = -1;

			ComponentDataList() = default;

			bool contains(IDType entityID) const
			{
				return indexOf(entityID) != npos;
			}

			T& add(IDType entityID, const T& data = T())
			{
				size_t insertIndex = npos;
				for (size_t i = 0; i < m_spans.size() || i == -1; ++i)
				{
					DataListSpan &span = m_spans[i];

					if (insertIndex != npos)
					{
						++span.listIndex;
						continue;
					}

					if (entityID > span.lastEntity + 1)
						continue;

					if (entityID == span.lastEntity + 1)
					{
						m_data.insert(m_data.begin() + span.entityListIndex(entityID), data);
						span.lastEntity = entityID;
						insertIndex = span.entityListIndex(entityID);

						// Join this span and the next if possible
						if (i + 1 < m_spans.size() && m_spans[i + 1].firstEntity == entityID + 1)
						{
							span.lastEntity = m_spans[i + 1].lastEntity;
							m_spans.erase(m_spans.begin() + i + 1);
							--i;
						}

						continue;
					}

					if (entityID >= span.firstEntity)
						return m_data[span.entityListIndex(entityID)];

					m_data.insert(m_data.begin() + span.listIndex, data);
					insertIndex = span.listIndex;

					if (entityID + 1 == span.firstEntity)
						span.firstEntity = entityID;
					else
						m_spans.emplace(m_spans.begin() + i, span.listIndex, entityID, entityID);
				}

				if (insertIndex != npos)
					return m_data[insertIndex];

				m_data.push_back(data);
				m_spans.emplace_back(m_data.size() - 1, entityID, entityID);
				return m_data.back();
			}

			T& at(IDType entityID)
			{
				size_t index = indexOf(entityID);

				if (index == npos)
					throw std::out_of_range("Entity ID not in component list");

				return m_data[index];
			}

			const T& at(IDType entityID) const
			{
				size_t index = indexOf(entityID);

				if (index == npos)
					throw std::out_of_range("Entity ID not in component list");

				return m_data[index];
			}

			void remove(IDType entityID)
			{
				size_t spanIndex = spanIndexOf(entityID);

				if (spanIndex == npos)
					return;

				m_data.erase(m_data.begin() + m_spans[spanIndex].entityListIndex(entityID));

				for (size_t i = spanIndex + 1; i < m_spans.size(); ++i)
				{
					--m_spans[i].listIndex;
				}

				if (m_spans[spanIndex].range() == 0)
				{
					m_spans.erase(m_spans.begin() + spanIndex);
					return;
				}

				if (m_spans[spanIndex].firstEntity == entityID)
				{
					++m_spans[spanIndex].firstEntity;
					return;
				}

				if (m_spans[spanIndex].lastEntity == entityID)
				{
					--m_spans[spanIndex].firstEntity;
					return;
				}

				// Split the span into two
				m_spans.emplace(m_spans.begin() + spanIndex + 1, m_spans[spanIndex].listIndex, m_spans[spanIndex].firstEntity, entityID - 1);
				m_spans.emplace(m_spans.begin() + spanIndex + 2, m_spans[spanIndex].entityListIndex(entityID), entityID + 1, m_spans[spanIndex].lastEntity);
				m_spans.erase(m_spans.begin() + spanIndex);
			}

			T& operator[](IDType entityID)
			{
				size_t index = indexOf(entityID);

				if (index != npos)
					return m_data[index];

				return add(entityID);
			}

		private:
			size_t indexOf(IDType entityID) const
			{
				size_t spanIndex = spanIndexOf(entityID);

				if (spanIndex == npos)
					return npos;

				return m_spans[spanIndex].entityListIndex(entityID);
			}

			size_t spanIndexOf(IDType entityID) const
			{
				size_t start = 0, end = m_spans.size();

				while (start != end)
				{
					size_t mid = (start + end) / 2;

					if (entityID < m_spans[mid].firstEntity)
					{
						end = mid;
						continue;
					}

					if (entityID > m_spans[mid].lastEntity)
					{
						start = mid + 1;
						continue;
					}

					return mid;
				}

				return npos;
			}
		};

		template <typename T>
		class Component
		{
		protected:
			using ComponentDataType = T;

			ComponentDataList<T> m_entityData;

			Component() = default;

		public:
			Component(const Component&) = delete;

			static Component& Get()
			{
				static Component s_instance;
				return s_instance;
			}

			void attachToEntity(Entity &entity, const T& val)
			{
				if (isAttachedToEntity(entity))
					return;

				m_entityData[entity.getEntityID()] = val;
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