#pragma once
#include "Math/Types.h"

namespace Core {
	class Transform {
	public:
		Mat4 GetMatrix() const;

		void TranslateLocal(const Vec3& delta);
		void TranslateWorld(const Vec3& delta);

		void Rotate(const Vec3& delta);
		void RotatePitch(float radians);
		void RotateYaw(float radians);
		void SetRotation(const Vec3& rotate);

		Vec3 Forward() const;
		Vec3 Right() const;
		Vec3 Up() const;

		Vec3 position{ 0.0f };
		Vec3 rotation{ 0.0f };
		Vec3 scale{ 1.0f };

	private:
		mutable Mat4 m_WorldMatrix{ 1.0f };
		mutable bool m_Dirty = true;
	};
}