#include "Rendering/Mesh.h"
#include <GL/glew.h>

namespace Core {
	Mesh::Mesh(const Vertex* vertices, size_t vertices_size, const std::uint32_t* indices, size_t indices_size)
		:m_IndexCount(static_cast<std::uint32_t > (indices_size / sizeof(std::uint32_t))) {
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

		SetLayout();
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: m_VAO(other.m_VAO), m_VBO(other.m_VBO), m_EBO(other.m_EBO), m_IndexCount(other.m_IndexCount) {
		other.m_VAO = other.m_VBO = other.m_EBO = 0;
		other.m_IndexCount = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept {
		if (this != &other) {
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
			glDeleteBuffers(1, &m_EBO);

			m_VAO = other.m_VAO;
			m_VBO = other.m_VBO;
			m_EBO = other.m_EBO;
			m_IndexCount = other.m_IndexCount;

			other.m_VAO = other.m_VBO = other.m_EBO = 0;
			other.m_IndexCount = 0;
		}

		return *this;
	}

	Mesh::~Mesh() noexcept {
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	void Mesh::SetLayout() {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
		glEnableVertexAttribArray(1);
	}

	void Mesh::Bind() const {
		glBindVertexArray(m_VAO);
	}

	void Mesh::Unbind() const {
		glBindVertexArray(0);
	}

	void Mesh::Draw() const {
		Bind();
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	}
}