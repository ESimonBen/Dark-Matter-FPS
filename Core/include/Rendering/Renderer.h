#pragma once
#include "Scene/Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"

namespace Core {
	class Renderer {
	public:
		static void BeginScene(Camera& camera, float alpha);
		static void Submit(const Mesh& mesh, const ShaderProgram& program, const Mat4& transform);
		static void EndScene();
	};
}