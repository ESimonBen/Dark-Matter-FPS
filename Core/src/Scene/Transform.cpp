#include "Scene/Transform.h"
#include "Scene/Entity.h"

namespace Core {
	void Transform::TranslateLocal(const Vec3& delta) {
		m_Position += (Forward() * delta.z) + (Right() * delta.x) + (Up() * delta.y);
		m_Dirty = true;
	}

	void Transform::TranslateWorld(const Vec3& delta) {
		m_Position += delta;
		m_Dirty = true;
	}

	void Transform::Rotate(const Vec3& deltaRadians) {
		/*m_Rotation += delta;*/
		Quat qPitch = glm::angleAxis(deltaRadians.x, Right());
		Quat qYaw = glm::angleAxis(deltaRadians.y, Vec3{0.0f, 1.0f, 0.0f});
		Quat qRoll = glm::angleAxis(deltaRadians.z, Forward());

		m_Rotation = glm::normalize(qYaw * qPitch * qRoll * m_Rotation);

		m_Dirty = true;
	}

	void Transform::RotatePitch(float radians) {
		/*m_Rotation.x += radians;
		const float limit = glm::half_pi<float>() - 0.001f;
		m_Rotation.x = glm::clamp(m_Rotation.x, -limit, limit);*/
		Quat quat = glm::angleAxis(radians, Right());
		m_Rotation = glm::normalize(quat * m_Rotation);
		m_Dirty = true;
	}

	void Transform::RotateYaw(float radians) {
		/*m_Rotation.y += radians;*/
		Quat quat = glm::angleAxis(radians, Vec3{ 0.0f, 1.0f, 0.0f });
		m_Rotation = glm::normalize(quat * m_Rotation);
		m_Dirty = true;
	}

	

	Vec3 Transform::Forward() const {/*
		return glm::normalize(Vec3{
			sin(m_Rotation.y) * cos(m_Rotation.x),
			sin(m_Rotation.x),
			-cos(m_Rotation.y) * cos(m_Rotation.x)
		});*/

		return glm::normalize(m_Rotation * Vec3{ 0.0f, 0.0f, -1.0f });
	}

	Vec3 Transform::Right() const {
		/*return glm::normalize(glm::cross(Forward(), Vec3{ 0, 1, 0 }));*/
		return glm::normalize(m_Rotation * Vec3{ 1.0f, 0.0f, 0.0f });
	}

	Vec3 Transform::Up() const {
		/*return glm::normalize(glm::cross(Right(), Forward()));*/
		return glm::normalize(m_Rotation * Vec3{ 0.0f, 1.0f, 0.0f });
	}

	Vec3 Transform::Position() {
		return m_Position;
	}

	Vec3 Transform::PreviousPosition() {
		return m_PreviousPosition;
	}

	Quat Transform::PreviousRotation() {
		return m_PreviousRotation;
	}

	Quat Transform::Rotation() {
		return m_Rotation;
	}

	Vec3 Transform::Scale() {
		return m_Scale;
	}

	void Transform::SetPosition(const Vec3& position) {
		m_Position = position;
		m_Dirty = true;
	}

	void Transform::SetPreviousPosition(const Vec3& position) {
		m_PreviousPosition = position;
	}

	void Transform::SetRotation(const Quat& rotate) {
		m_Rotation = rotate;
		m_Dirty = true;
	}

	void Transform::SetPreviousRotation(const Quat& rotate) {
		m_PreviousRotation = rotate;
	}

	void Transform::SetScale(const Vec3& scale) {
		m_Scale = scale;
		m_Dirty = true;
	}

	EntityID Transform::GetOwner() const {
		return m_Owner;
	}

	void Transform::SetOwner(EntityID id) {
		m_Owner = id;
	}

	Mat4 Transform::GetMatrix() const {
		Mat4 m{ 1.0f }; // Identity Matrix

		m = glm::translate(m, m_Position);
		m *= glm::mat4_cast(m_Rotation);
		m = glm::scale(m, m_Scale);

		if (m_Parent) {
			return m_Parent->GetMatrix() * m;
		}

		m_WorldMatrix = m;
		m_Dirty = false;
		return m_WorldMatrix;
	}

	Mat4 Transform::GetInterpolatedMatrix(float alpha) const {

		Vec3 interpPosition = glm::mix(m_PreviousPosition, m_Position, alpha);
		Quat interpRotation = glm::slerp(m_PreviousRotation, m_Rotation, alpha);

		Mat4 translate = glm::translate(Mat4{ 1.0f }, interpPosition);
		Mat4 rotation = glm::mat4_cast(interpRotation);
		Mat4 scale = glm::scale(Mat4{ 1.0f }, m_Scale);

		Mat4 finalMat = translate * rotation * scale;

		if (m_Parent) {
			return m_Parent->GetInterpolatedMatrix(alpha) * finalMat;
		}

		return finalMat;
	}

	void Transform::AddChild(Entity child) {
		m_Children.push_back(child.GetID());
		child.GetTransform().m_Parent = this;
	}

	void Transform::RemoveChild(Entity child) {
		m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child.GetID()), m_Children.end());
		child.GetTransform().m_Parent = nullptr;
	}

	Transform* Transform::GetParent() {
		return m_Parent;
	}
}