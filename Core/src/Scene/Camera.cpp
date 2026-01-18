#include "Scene/Camera.h"
#include <cmath>

namespace Core {
	Camera::Camera(const Vec3& position, const Vec3& up, float fov, float width, float height, float near_plane, float far_plane) 
		: m_Position(position), m_FOV(fov), m_AspectRatio(width / height), m_Near(near_plane), m_Far(far_plane)
	{
		m_Front = CreateForward(m_Pitch, m_Yaw);
		m_Right = glm::normalize(glm::cross(m_Front, up));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_Projection = glm::perspective(fov, m_AspectRatio, near_plane, far_plane);
	}

	void Camera::AdjustPitch(float pitch) {
		m_Pitch += pitch;
		m_Pitch = glm::clamp(m_Pitch, -(glm::half_pi<float>() - (glm::half_pi<float>() / 90.0f)), (glm::half_pi<float>() - (glm::half_pi<float>() / 90.0f)));
		m_Front = CreateForward(m_Pitch, m_Yaw);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void Camera::AdjustYaw(float yaw) {
		m_Yaw += yaw;
		m_Front = CreateForward(m_Pitch, m_Yaw);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void Camera::Translate(const Vec3& translate, float dt) {
		m_Position += (translate * dt);
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void Camera::SetAspectRatio(float width, float height) {
		m_AspectRatio = width / height;
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_Near, m_Far);
	}

	Mat4 Camera::View() const {
		return m_View;
	}

	Mat4 Camera::Projection() const {
		return m_Projection;
	}

	Vec3 Camera::Position() const {
		return m_Position;
	}

	Vec3 Camera::Front() const {
		return m_Front;
	}

	Vec3 Camera::Right() const {
		return m_Right;
	}

	Vec3 Camera::Up() const {
		return m_Up;
	}

	Vec3 Camera::CreateForward(float pitch, float yaw) {
		return glm::normalize(Vec3{ std::sin(yaw) * std::cos(pitch), std::sin(pitch), -std::cos(yaw) * std::cos(pitch) });
	}
}