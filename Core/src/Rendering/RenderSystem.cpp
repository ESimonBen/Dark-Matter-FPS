#include "Rendering/RenderSystem.h"


namespace Core {
	void RenderSystem::OnRender(Scene& scene, Renderer& renderer, float alpha) {
		Camera& camera = scene.GetActiveCamera();
		auto& meshComponents = scene.MeshComponents();

		renderer.BeginScene(camera);

		for (size_t i = 0; i < meshComponents.Size(); i++) {
			EntityID id = meshComponents.Entities().at(i);
			auto& meshComponent = meshComponents.Components().at(i);

			Transform& transform = scene.Transforms().Get(id);

			renderer.Submit(meshComponent.m_Mesh, *meshComponent.m_Program, transform.GetInterpolatedMatrix(alpha, true));
		}

		renderer.EndScene();
	}
}