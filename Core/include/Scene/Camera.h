#pragma once
#include "Math/Types.h"
#include "Transform.h"

namespace Core {
	class Camera {
	public:
		Camera(Transform& transform, float fov, float width, float height, float near_plane, float far_plane);

		void SetAspectRatio(float width, float height);

		Mat4 View() const;
		Mat4 Projection() const;
		Transform& GetTransform();
		const Transform& GetTransform() const;

	private:
		Transform& m_Transform;
		float m_FOV;
		float m_AspectRatio;
		float m_Near, m_Far;
		mutable Mat4 m_Projection;
		mutable bool m_ProjDirty = true;
	};
}