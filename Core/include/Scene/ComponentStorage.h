#pragma once
#include <vector>
#include <unordered_map>
#include <cassert>
#include <Debug/Assert.h>
#include <Scene/Entity.h>

namespace Core {
	template <typename T>
	class ComponentStorage {
	public:
		T& Add(EntityID entity, T component) {
			CORE_ASSERT(!Has(entity) && "Entity already has this component")

			size_t index = m_Components.size();
			m_Components.push_back(std::move(component));
			m_Entities.push_back(entity);
			m_Lookup[entity] = index;

			return m_Components.back();
		}

		void Remove(EntityID entity) {
			CORE_ASSERT(Has(entity) && "This component does not exist")

			size_t index = m_Lookup[entity];
			size_t lastIndex = m_Components.size() - 1;
			EntityID lastEntity = m_Entities[lastIndex];

			m_Components[index] = std::move(m_Components[lastIndex]);
			m_Entities[index] = lastEntity;
			m_Lookup[lastEntity] = index;

			m_Components.pop_back();
			m_Entities.pop_back();
			m_Lookup.erase(entity);
		}

		bool Has(EntityID entity) const {
			return m_Lookup.find(entity) != m_Lookup.end();
		}

		T& Get(EntityID entity) {
			CORE_ASSERT(Has(entity) && "Component does not exist")

			return m_Components[m_Lookup[entity]];
		}

		const T& Get(EntityID entity) const {
			CORE_ASSERT(Has(entity) && "Component does not exist")
				
			return m_Components.at(m_Lookup.at(entity));
		}

		const std::vector<T>& Components() const {
			return m_Components;
		}

		const std::vector<EntityID>& Entities() const{
			return m_Entities;
		}

		size_t Size() const {
			return m_Components.size();
		}

	private:
		std::vector<T> m_Components;
		std::vector<EntityID> m_Entities;
		std::unordered_map<EntityID, size_t> m_Lookup;
	};
}