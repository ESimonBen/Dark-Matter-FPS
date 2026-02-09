#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Debug/Assert.h"

namespace Core{
	Entity::Entity(EntityID id, Scene* scene)
		: m_ID(id), m_Scene(scene)
	{}

	EntityID Entity::GetID() const {
		return m_ID;
	}

	bool Entity::IsValid() const {
		return m_Scene && m_Scene->IsEntityAlive(m_ID);
	}

	Scene* Entity::GetScene() const {
		return m_Scene;
	}

	bool Entity::operator==(const Entity& other) const {
		return m_ID == other.m_ID && m_Scene == other.m_Scene;
	}

	bool Entity::operator!=(const Entity& other) const {
		return !(*this == other);
	}

	Transform& Entity::GetTransform() {
		CORE_ASSERT(IsValid())
		return *m_Scene->GetTransform(*this);
	}

	bool Entity::HasMesh() const {
		CORE_ASSERT(IsValid());
		return m_Scene->HasMesh(m_ID);
	}
}