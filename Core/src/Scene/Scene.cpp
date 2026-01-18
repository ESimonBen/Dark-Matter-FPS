#include "Scene/Scene.h"

namespace Core {
	Scene::Scene() {
		// Nothing here for now
	}

	Entity Scene::CreateEntity() {
		EntityID id = m_NextID++;
		m_Transforms[id] = Transform{};
		return Entity{ id, this };
	}

	void Scene::OnUpdate(float dt) {
		// Nothing for now
	}

	void Scene::OnRender(Renderer& renderer, Camera& camera) {
		renderer.BeginScene(camera);

		for (auto& [id, transform] : m_Transforms) {
			if (m_Meshes.count(id)) {
				renderer.Submit(*m_Meshes[id], *m_Programs[id], transform.GetMatrix());
			}
		}

		renderer.EndScene();
	}

	Transform& Scene::GetTransform(Entity entity) {
		return m_Transforms[entity.GetID()];
	}

	void Scene::AttachMesh(Entity entity, std::unique_ptr<Mesh> mesh, std::unique_ptr<ShaderProgram> program) {
		EntityID id = entity.GetID();
		m_Meshes[id] = std::move(mesh);
		m_Programs[id] = std::move(program);
	}
}