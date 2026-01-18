#include "Rendering/Shader.h"
#include <GL/glew.h>
#include <fstream>
#include <sstream>

namespace Core {
	Shader::Shader(const std::string& path, ShaderType type) 
		: m_Type(type) {
		m_Handle = glCreateShader(NativeType());
		std::string src = LoadShaderSource(path);
		const char* strings[] = {src.data()};
		const int lengths[] = { (int)src.size() };

		glShaderSource(m_Handle, 1, strings, lengths);
		glCompileShader(m_Handle);

		GLint success;
		glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint logLength;
			glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &logLength);

			std::string log(logLength, '\0');
			glGetShaderInfoLog(m_Handle, logLength, nullptr, log.data());

			glDeleteShader(m_Handle);
			m_Handle = 0;

			throw std::runtime_error("Shader compilation failed:\n" + log);
		}
	}

	Shader::Shader(Shader&& other) noexcept
		: m_Handle(other.m_Handle), m_Type(other.m_Type){
		other.m_Handle = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept {
		if (this != &other) {
			glDeleteShader(m_Handle);
			m_Handle = other.m_Handle;
			m_Type = other.m_Type;
			other.m_Handle = 0;
		}

		return *this;
	}

	Shader::~Shader() noexcept {
		if (m_Handle != 0) {
			glDeleteShader(m_Handle); // Will improve upon
		}
	}

	GLHandle Shader::Handle() const {
		return m_Handle;
	}

	unsigned int Shader::NativeType() const {
		switch (m_Type) {
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		default:
			throw std::runtime_error("Shader type not recognized");
		}
	}

	ShaderType Shader::Type() const {
		return m_Type;
	}

	// May move this to a Core::IO namespace along with other file
	// loaders in the future
	std::string Shader::LoadShaderSource(const std::string& path) {
		std::ifstream file{ path, std::ios::in | std::ios::binary };

		if (!file.is_open()) {
			throw std::runtime_error("Could not open file");
		}

		std::ostringstream oss;
		oss << file.rdbuf();

		return oss.str();
	}
}