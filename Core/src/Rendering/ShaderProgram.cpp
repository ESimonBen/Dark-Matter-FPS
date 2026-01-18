#include "Rendering/ShaderProgram.h"
#include "GL/glew.h"
#include <stdexcept>

namespace Core {
	ShaderProgram::ShaderProgram() 
		: m_Handle(glCreateProgram()) 
	{}

	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
		: m_Handle(other.m_Handle) {
		other.m_Handle = 0;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
		if (this != &other) {
			glDeleteProgram(m_Handle);
			m_Handle = other.m_Handle;
			other.m_Handle = 0;
		}

		return *this;
	}

	ShaderProgram::~ShaderProgram() noexcept {
		glDeleteProgram(m_Handle);
	}

	void ShaderProgram::SetVec3(const std::string& name, const Vec3& data)  const {
		GLint location = glGetUniformLocation(m_Handle, name.c_str());
		glUniform3f(location, data.x, data.y, data.z);
	}

	void ShaderProgram::SetVec4(const std::string& name, const Vec4& data) const {
		GLint location = glGetUniformLocation(m_Handle, name.c_str());
		glUniform4f(location, data.x, data.y, data.z, data.w);
	}

	void ShaderProgram::SetMat4(const std::string& name, const Mat4& data) const {
		GLint location = glGetUniformLocation(m_Handle, name.c_str());

		// GLM has a value pointer method that returns the pointer to that variable (for example this Mat4)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(data));
	}

	void ShaderProgram::Attach(const Shader& shader) {
		glAttachShader(m_Handle, (GLuint)shader.Handle());
	}

	void ShaderProgram::Link() {
		glLinkProgram(m_Handle);

		GLint success;
		glGetProgramiv(m_Handle, GL_LINK_STATUS, &success);
		if (!success) {
			GLint logLength;
			glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &logLength);

			std::string log(logLength, '\0');
			glGetProgramInfoLog(m_Handle, logLength, nullptr, log.data());

			throw std::runtime_error("Program linking failed:\n" + log);
		}
	}

	void ShaderProgram::Validate() const {
		glValidateProgram(m_Handle);
	}

	void ShaderProgram::Bind() const {
		glUseProgram(m_Handle);
	}

	void ShaderProgram::Unbind() const {
		glUseProgram(0);
	}

	GLHandle ShaderProgram::Handle() const {
		return m_Handle;
	}
}