#pragma once
#include "Scripting/ScriptableEntity.h"
#include "Physics/PhysicsComponent.h"

namespace DarkMatter {
	class PlayerControllerScript : public Core::ScriptableEntity {
	public:
		void OnCreate() override;
		void OnFixedUpdate(float dt) override;

	private:
		Core::Vec3 m_GroundNormal{ 0.0f, 1.0f, 0.0f };
		Core::Transform* m_Transform = nullptr;
		Core::PhysicsComponent* m_PhysicsComponent = nullptr;
		float m_Speed = 5.0f;
		float m_JumpSpeed = 5.0f;
		bool m_IsGrounded = false;
		bool m_JumpedLastFrame = false;
		float m_GroundAcceleration = 4.0f;
		float m_AirAcceleration = 1.5f;
	};
}