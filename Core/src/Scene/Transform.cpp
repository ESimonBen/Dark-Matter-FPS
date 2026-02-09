#include "Scene/Transform.h"

namespace Core {
	void Transform::TranslateLocal(const Vec3& delta) {
		position += (Forward() * delta.z) + (Right() * delta.x) + (Up() * delta.y);
		m_Dirty = true;
	}

	void Transform::TranslateWorld(const Vec3& delta) {
		position += delta;
		m_Dirty = true;
	}

	void Transform::Rotate(const Vec3& delta) {
		rotation += delta;
		m_Dirty = true;
	}

	void Transform::RotatePitch(float radians) {
		rotation.x += radians;
		const float limit = glm::half_pi<float>() - 0.001f;
		rotation.x = glm::clamp(rotation.x, -limit, limit);
		m_Dirty = true;
	}

	void Transform::RotateYaw(float radians) {
		rotation.y += radians;
		m_Dirty = true;
	}

	void Transform::SetRotation(const Vec3& rotate) {
		rotation = rotate;
		m_Dirty = true;
	}

	Vec3 Transform::Forward() const {
		return glm::normalize(Vec3{
			sin(rotation.y) * cos(rotation.x),
			sin(rotation.x),
			-cos(rotation.y) * cos(rotation.x)
		});
	}

	Vec3 Transform::Right() const {
		return glm::normalize(glm::cross(Forward(), Vec3{ 0, 1, 0 }));
	}

	Vec3 Transform::Up() const {
		return glm::normalize(glm::cross(Right(), Forward()));
	}

	Mat4 Transform::GetMatrix() const {
		Mat4 m{ 1.0f }; // Identity Matrix

		m = glm::translate(m, position);

		m = glm::rotate(m, rotation.y, Vec3{ 0.0f, 1.0f, 0.0f });
		m = glm::rotate(m, rotation.x, Vec3{ 1.0f, 0.0f, 0.0f });
		m = glm::rotate(m, rotation.z, Vec3{ 0.0f, 0.0f, 1.0f });

		m = glm::scale(m, scale);

		m_WorldMatrix = m;
		m_Dirty = false;
		return m_WorldMatrix;
	}
}