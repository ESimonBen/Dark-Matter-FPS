#pragma once
#include "Math/Types.h"
#include <vector>

namespace Core {
	// To reduce circula dependencies
	class Entity;
	using EntityID = uint32_t;

	class Transform {
	public:
		Mat4 GetMatrix() const;
		Mat4 GetInterpolatedMatrix(float alpha) const;

		void AddChild(Entity child);
		void RemoveChild(Entity child);
		Transform* GetParent();

		void TranslateLocal(const Vec3& delta);
		void TranslateWorld(const Vec3& delta);

		void Rotate(const Vec3& delta);
		void RotatePitch(float radians);
		void RotateYaw(float radians);

		Vec3 Forward() const;
		Vec3 Right() const;
		Vec3 Up() const;

		Vec3 Position();
		Vec3 PreviousPosition();
		Quat PreviousRotation();
		Quat Rotation();
		Vec3 Scale();
		void SetPosition(const Vec3& position);
		void SetPreviousPosition(const Vec3& position);
		void SetRotation(const Quat& rotate);
		void SetPreviousRotation(const Quat& rotate);
		void SetScale(const Vec3& scale);

		EntityID GetOwner() const;
		void SetOwner(EntityID id);

	private:
		mutable Mat4 m_WorldMatrix{ 1.0f };
		Quat m_Rotation{ 1, 0, 0, 0 };
		Quat m_PreviousRotation{ 1, 0, 0, 0 };
		Vec3 m_Position{ 0.0f };
		Vec3 m_PreviousPosition{ 0.0f };
		Vec3 m_Scale{ 1.0f };
		mutable bool m_Dirty = true;

		Transform* m_Parent = nullptr;
		std::vector<EntityID> m_Children;
		EntityID m_Owner;
	};
}