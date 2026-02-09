#include "Scene/Scene.h"
#include "Scripting/ScriptableEntity.h"
#include "Debug/Assert.h"

namespace Core {
	Scene::Scene() {
		// Nothing here for now
	}

	Entity Scene::CreateEntity() {
		EntityID id = m_NextID++;
		m_Transforms[id] = Transform{};
		return Entity{ id, this };
	}

	Camera& Scene::CreateCamera(Entity entity, float fov, float width, float height, float near_plane, float far_plane) {
		EntityID id = entity.GetID();
		Transform& transform = m_Transforms[id];
		m_Cameras[id] = std::make_unique<Camera>(transform, fov, width, height, near_plane, far_plane);

		return *m_Cameras[id];
	}

	void Scene::SetActiveCamera(Entity entity) {
		m_ActiveCamera = entity.GetID();
	}

	Camera* Scene::GetActiveCamera() {
		auto it = m_Cameras.find(m_ActiveCamera);
		return it != m_Cameras.end() ? it->second.get() : nullptr;
	}

	void Scene::OnUpdate(float dt) {
		for (auto& [id, scripts] : m_Scripts) {
			for (auto& script : scripts) {
				if (!script.m_Initialized) {
					script.m_Instance = script.Instantiate();
					script.m_Instance->m_Entity = Entity{ id, this };
					script.m_Instance->m_Scene = this;
					script.m_Instance->OnCreate();
					script.m_Initialized = true;
				}

				script.m_Instance->OnUpdate(dt);
			}
		}
	}

	void Scene::OnEvent(Event& event) {
		for (auto& [id, scripts] : m_Scripts) {
			for (auto& script : scripts) {
				if (script.m_Initialized) {
					script.m_Instance->OnEvent(event);
				}
			}
		}
	}

	void Scene::OnRender(Renderer& renderer) {
		Camera* camera = GetActiveCamera();

		if (!camera) {
			return;
		}

		renderer.BeginScene(*camera);

		for (auto& [id, component] : m_MeshComponents) {
			auto it = m_Transforms.find(id);
			if (it == m_Transforms.end()) {
				continue;
			}

			renderer.Submit(*(component->m_Mesh), *(component->m_Program), it->second.GetMatrix());
		}

		renderer.EndScene();
	}

	Transform* Scene::GetTransform(Entity entity) {
		auto it = m_Transforms.find(entity.GetID()); // Get the iterator position of the Entity
		if (it == m_Transforms.end()) {
			return nullptr;
		}

		return &it->second; // Return the Transform of the Entity
	}

	Camera* Scene::GetCamera(Entity entity) {
		auto it = m_Cameras.find(entity.GetID());
		if (it == m_Cameras.end()) {
			return nullptr;
		}

		return it->second.get();
	}

	void Scene::AttachMesh(Entity entity, std::unique_ptr<Mesh> mesh, std::shared_ptr<ShaderProgram> program) {
		CORE_ASSERT(entity.IsValid())
		EntityID id = entity.GetID();
		m_MeshComponents.emplace(id, std::make_unique<MeshComponent>( std::move(mesh), program ));
	}

	bool Scene::HasMesh(EntityID id) const {
		return m_MeshComponents.find(id) != m_MeshComponents.end();
	}

	void Scene::OnWindowResize(WindowResizeEvent& resize) {
		if (Camera* cam = GetActiveCamera()) {
			cam->SetAspectRatio(resize.GetWidth(), resize.GetHeight());
		}
	}

	void Scene::SetCursorLocked(bool mode){
		m_CursorLocked = mode;
	}

	bool Scene::IsCursorLocked() const {
		return m_CursorLocked;
	}

	bool Scene::ConsumeCursorDirty() {
		if (m_CursorDirty) {
			m_CursorDirty = false;
			return true;
		}

		return false;
	}

	void Scene::DestroyEntity(Entity entity){
		CORE_ASSERT(entity.IsValid())
		EntityID id = entity.GetID();

		if (auto it = m_Scripts.find(id); it != m_Scripts.end()) {
			for (auto& script : it->second) {
				if (script.m_Initialized) {
					script.m_Instance->OnDestroy();
				}
			}
			m_Scripts.erase(it);
		}

		if (m_ActiveCamera == id) {
			m_ActiveCamera = 0;
		}

		m_Transforms.erase(id);
		m_MeshComponents.erase(id);
		m_Cameras.erase(id);
	}

	bool Scene::IsEntityAlive(EntityID id) const {
		return m_Transforms.find(id) != m_Transforms.end();
	}
}