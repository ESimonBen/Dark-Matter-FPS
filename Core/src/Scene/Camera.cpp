#include "Scene/Camera.h"
#include <cmath>

namespace Core {
	Camera::Camera(Transform& transform, float fov, float width, float height, float near_plane, float far_plane)
		: m_Transform(transform), m_FOV(fov), m_AspectRatio(width / height), m_Near(near_plane), m_Far(far_plane) {
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
	}

	void Camera::SetAspectRatio(float width, float height) {
		m_AspectRatio = width / height;
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
	}

	Mat4 Camera::View() const {
		const Vec3 position = m_Transform.position;
		Vec3 forward = m_Transform.Forward();
		Vec3 up = m_Transform.Up();

		return glm::lookAt(position, position + forward, up);
	}

	Mat4 Camera::Projection() const {
		if (m_ProjDirty) {
			m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
			m_ProjDirty = false;
		}

		return m_Projection;
	}
	Transform& Camera::GetTransform() {
		return m_Transform;
	}
	const Transform& Camera::GetTransform() const {
		return m_Transform;
	}
}