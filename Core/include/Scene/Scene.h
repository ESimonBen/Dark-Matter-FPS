#pragma once
#include <unordered_map>
#include <memory>
#include "Entity.h"
#include "Transform.h"
#include "MeshComponent.h"
#include "Rendering/Renderer.h"
#include "Scripting/CameraControllerScript.h"
#include "Scripting/ScriptComponent.h"
#include "Physics/PhysicsComponent.h"


namespace Core {
	class ScriptableEntity;

	class Scene {
	public:
		Scene();

		Entity CreateEntity();
		Camera& CreateCamera(Entity entity, float fov, float width, float height, float near_plane, float far_plane);
		void SetActiveCamera(Entity entity);
		Camera* GetActiveCamera();

		template <typename T>
		void AttachScript(Entity entity) {
			static_assert(std::is_base_of_v<ScriptableEntity, T>, "T must derive from ScriptableEntity");
				
			EntityID id = entity.GetID();
			auto& scripts = m_Scripts[id];

			ScriptComponent& sc = scripts.emplace_back();
			sc.Instantiate = []() {
				return std::make_unique<T>();
			};
		}

		void OnUpdate(float dt);
		void OnEvent(Event& event);
		void OnRender(Renderer& renderer);

		Transform* GetTransform(Entity entity);
		Camera* GetCamera(Entity entity);
		void AttachMesh(Entity entity, std::unique_ptr<Mesh> mesh, std::shared_ptr<ShaderProgram> program);
		void AttachPhysicsBox(Entity entity, const Vec3& halfExtent, bool isStatic, Quat rotation);
		bool HasMesh(EntityID id) const;
		void OnWindowResize(WindowResizeEvent& resize);

		void SetCursorLocked(bool mode);
		bool IsCursorLocked() const;
		bool ConsumeCursorDirty();

		void DestroyEntity(Entity entity);
		bool IsEntityAlive(EntityID id) const;

	private:
		std::unordered_map<EntityID, Transform> m_Transforms;
		std::unordered_map<EntityID, std::unique_ptr<MeshComponent>> m_MeshComponents;
		std::unordered_map<EntityID, std::unique_ptr<Camera>> m_Cameras;
		std::unordered_map<EntityID, std::vector<ScriptComponent>> m_Scripts;
		std::unordered_map<EntityID, PhysicsComponent> m_Physics;
		EntityID m_NextID = 1;
		EntityID m_ActiveCamera = 0;
		bool m_CursorLocked = true;
		bool m_CursorDirty = true;
	};
}