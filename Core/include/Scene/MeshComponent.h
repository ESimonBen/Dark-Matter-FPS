#pragma once
#include <memory>
#include "Rendering/Mesh.h"
#include "Rendering/ShaderProgram.h"

namespace Core {
	struct MeshComponent {
		MeshComponent() = default;
		MeshComponent(Mesh mesh, std::shared_ptr<ShaderProgram> program)
			: m_Mesh(std::move(mesh)), m_Program(program)
		{}

		std::shared_ptr<ShaderProgram> m_Program;
		Mesh m_Mesh;
	};
}