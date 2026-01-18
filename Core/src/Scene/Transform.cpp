#include "Scene/Transform.h"

namespace Core {
	Mat4 Transform::GetMatrix() const {
		Mat4 m{ 1.0f }; // Identity Matrix

		m = glm::translate(m, position);

		m = glm::rotate(m, rotation.y, Vec3{ 0.0f, 1.0f, 0.0f });
		m = glm::rotate(m, rotation.x, Vec3{ 1.0f, 0.0f, 0.0f });
		m = glm::rotate(m, rotation.z, Vec3{ 0.0f, 0.0f, 1.0f });

		m = glm::scale(m, scale);

		return m;
	}
}