#pragma once
#include "Vertex.h"
#include <cstdint>

namespace Core {
	using GLHandle = unsigned int;

	class Mesh {
	public:
		Mesh(const Vertex* vertices, size_t vertices_size, const std::uint32_t* indices, size_t indices_size);
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;
		~Mesh() noexcept;

		void SetLayout();
		void Bind() const;
		void Unbind() const;
		void Draw() const;

	private:
		GLHandle m_VAO = 0;
		GLHandle m_VBO = 0;
		GLHandle m_EBO = 0;
		std::uint32_t m_IndexCount;
	};
}