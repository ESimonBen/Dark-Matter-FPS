#include "Scripting/ScriptableEntity.h"
#include "Scene/Scene.h"
#include "Debug/Assert.h"

namespace Core {
	void ScriptableEntity::OnCreate() {
		// Nothing to do for now
	}

	void ScriptableEntity::OnUpdate(float dt) {
		// Nothing to do for now
	}

	void ScriptableEntity::OnEvent(Event& event) {
		// Nothing to do for now
	}

	void ScriptableEntity::OnDestroy() {
		// Nothing to do for now
	}

	void ScriptableEntity::SetEntity(Entity entity, Scene* scene) {
		m_Entity = entity;
		m_Scene = scene;
	}

	Transform& ScriptableEntity::GetTransform() {
		return m_Scene->GetTransform(m_Entity);
	}

	Camera& ScriptableEntity::GetCamera() {
		return m_Scene->GetCamera(m_Entity);
	}

	Entity ScriptableEntity::GetEntity() const {
		return m_Entity;
	}
}