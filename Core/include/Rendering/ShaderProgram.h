#pragma once
#include "Shader.h"
#include "Math/Types.h"

namespace Core {
	class ShaderProgram {
	public:
		ShaderProgram();
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;
		~ShaderProgram() noexcept;

		void SetVec3(const std::string& name, const Vec3& data) const;
		void SetVec4(const std::string& name, const Vec4& data) const;
		void SetMat4(const std::string& name, const Mat4& data) const;

		void Attach(const Shader& shader);
		void Link();
		void Validate() const;
		void Bind() const;
		void Unbind() const;
		GLHandle Handle() const;

	private:
		GLHandle m_Handle;
	};
}