#include "Scene/Entity.h"

namespace Core{
	Entity::Entity(EntityID id, Scene* scene)
		: m_ID(id), m_Scene(scene)
	{}

	EntityID Entity::GetID() const {
		return m_ID;
	}

	bool Entity::IsValid() const {
		return m_Scene != nullptr && m_ID != 0;
	}
}