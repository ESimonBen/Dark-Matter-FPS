#pragma once
#include <string>


namespace Core {
	using GLHandle = unsigned int;

	enum class ShaderType {
		Vertex, Fragment
	};

	class Shader {
	public:
		Shader(const std::string& path, ShaderType type);
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
		~Shader() noexcept;

		GLHandle Handle() const;
		ShaderType Type() const;
		unsigned int NativeType() const;

	private:
		std::string LoadShaderSource(const std::string& path);

		GLHandle m_Handle = 0;
		ShaderType m_Type;
	};
}