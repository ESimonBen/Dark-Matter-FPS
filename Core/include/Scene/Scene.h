#pragma once
#include <unordered_map>
#include <memory>
#include "Entity.h"
#include "Transform.h"
#include "Rendering/Renderer.h"

namespace Core {
	class Scene {
	public:
		Scene();

		Entity CreateEntity();
		void OnUpdate(float dt);
		void OnRender(Renderer& renderer, Camera& camera);
		Transform& GetTransform(Entity entity);
		void AttachMesh(Entity entity, std::unique_ptr<Mesh> mesh, std::unique_ptr<ShaderProgram> program);

	private:
		EntityID m_NextID = 1;
		std::unordered_map<EntityID, Transform> m_Transforms;
		std::unordered_map<EntityID, std::unique_ptr<Mesh>> m_Meshes;
		std::unordered_map<EntityID, std::unique_ptr<ShaderProgram>> m_Programs;
	};
}