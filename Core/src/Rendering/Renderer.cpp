#include "Rendering/Renderer.h"
#include <GL/glew.h>

namespace Core {
	static Mat4 s_ViewProjection;
	static Camera* s_Camera;

	void Renderer::BeginScene(Camera& camera, float alpha) {
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		s_Camera = &camera;
		s_ViewProjection = s_Camera->Projection() * s_Camera->View(alpha);
	}

	void Renderer::Submit(const Mesh& mesh, const ShaderProgram& program, const Mat4& transform) {
		program.Bind();
		program.SetMat4("model", transform);
		program.SetMat4("viewProjection", s_ViewProjection);
		mesh.Draw();
	}

	void Renderer::EndScene() {
		s_Camera = nullptr;
	}
}