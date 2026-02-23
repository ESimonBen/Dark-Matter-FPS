#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include <cmath>

namespace Core {
	Camera::Camera(Entity entity, float fov, float width, float height, float near_plane, float far_plane)
		: m_ID(entity.GetID()), m_Scene(entity.GetScene()), m_FOV(fov), m_AspectRatio(width / height), m_Near(near_plane), m_Far(far_plane) {
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
	}

	void Camera::SetAspectRatio(float width, float height) {
		m_AspectRatio = width / height;
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
	}

	Mat4 Camera::View(float alpha) const {
		const Transform& transform = GetTransform();
		Mat4 world = transform.GetInterpolatedMatrix(alpha);
		return glm::inverse(world);
	}

	Mat4 Camera::Projection() const {
		if (m_ProjDirty) {
			m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
			m_ProjDirty = false;
		}

		return m_Projection;
	}
	Transform& Camera::GetTransform() {
		return m_Scene->GetTransform(Entity{m_ID, m_Scene});
	}
	const Transform& Camera::GetTransform() const {
		return m_Scene->GetTransform(Entity{ m_ID, m_Scene });
	}
}