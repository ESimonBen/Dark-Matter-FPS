#pragma once
#include "Math/Types.h"
#include "Transform.h"
#include "Entity.h"

namespace Core {
	class Camera {
	public:
		Camera(Entity entity, float fov, float width, float height, float near_plane, float far_plane);
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) noexcept = default;
		Camera& operator=(Camera&&) noexcept = default;


		void SetAspectRatio(float width, float height);

		Mat4 View() const;
		Mat4 Projection() const;
		Transform& GetTransform();
		const Transform& GetTransform() const;

	private:
		mutable Mat4 m_Projection;
		Scene* m_Scene;
		EntityID m_ID;
		float m_FOV;
		float m_AspectRatio;
		float m_Near, m_Far;
		mutable bool m_ProjDirty = true;
	};
}