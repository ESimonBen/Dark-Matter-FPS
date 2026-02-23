#pragma once
#include "Scripting/ScriptableEntity.h"
#include "Physics/PhysicsComponent.h"

namespace DarkMatter {
	class PlayerControllerScript : public Core::ScriptableEntity {
	public:
		void OnCreate() override;
		void OnFixedUpdate(float dt) override;

	private:
		Core::Transform* m_Transform = nullptr;
		Core::PhysicsComponent* m_PhysicsComponent = nullptr;
		float m_Speed = 5.0f;
		float m_JumpSpeed = 5.0f;
		bool m_IsGrounded = false;
		bool m_JumpedLastFrame = false;
	};
}