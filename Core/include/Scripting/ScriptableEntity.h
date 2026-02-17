#pragma once
#include "Scene/Entity.h"
#include "Events/Event.h"
#include "Scene/Camera.h"

namespace Core {
	class Transform;

	class ScriptableEntity {
		friend class Scene;
		friend class ScriptSystem;

	public:
		virtual ~ScriptableEntity() = default;

		virtual void OnCreate();
		virtual void OnUpdate(float dt);
		virtual void OnFixedUpdate(float dt);
		virtual void OnEvent(Event& event);
		virtual void OnDestroy();

	protected:
		Transform& GetTransform();
		Camera& GetCamera();
		Entity GetEntity() const;
		

		void SetEntity(Entity entity, Scene* scene);

		Entity m_Entity;
		Scene* m_Scene = nullptr;
	};
}