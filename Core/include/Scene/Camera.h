#pragma once
#include "Math/Types.h"

namespace Core {
	class Camera {
	public:
		Camera(const Vec3& position, const Vec3& up, float fov, float width, float height, float near_plane, float far_plane);
		void AdjustPitch(float pitch);
		void AdjustYaw(float yaw);
		void Translate(const Vec3& translate, float dt);
		void SetAspectRatio(float width, float height);

		Mat4 View() const;
		Mat4 Projection() const;
		Vec3 Position() const;
		Vec3 Front() const;
		Vec3 Right() const;
		Vec3 Up() const;

	private:
		Vec3 CreateForward(float pitch, float yaw);

		Mat4 m_View;
		Mat4 m_Projection;
		Vec3 m_Position;
		Vec3 m_Front;
		Vec3 m_Right;
		const Vec3 m_WorldUp{ 0.0f, 1.0f, 0.0f };
		Vec3 m_Up;
		float m_FOV;
		float m_AspectRatio;
		float m_Near, m_Far;
		float m_Pitch = 0;
		float m_Yaw = 0;
	};
}