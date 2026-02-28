#include "Scripts/PlayerControllerScript.h"
#include "Input/Input.h"
#include "Scene/Scene.h"
#include "Physics/PhysicsManager.h"

namespace DarkMatter {
	void PlayerControllerScript::OnCreate() {
		m_Transform = &GetTransform();

		auto& physComponents = m_Scene->PhysicsComponents();
		
		if (physComponents.Has(m_Entity.GetID())) {
			m_PhysicsComponent = &physComponents.Get(m_Entity.GetID());
		}
	}

	void PlayerControllerScript::OnFixedUpdate(float dt) {
		if (!m_Transform) {
			return;
		}

		Core::Vec3 moveDir{ 0.0f };

		if (Core::Input::IsKeyDown(Core::Key::W)) {
			moveDir.z += 1.0f;
		}

		if (Core::Input::IsKeyDown(Core::Key::A)) {
			moveDir.x -= 1.0f;
		}

		if (Core::Input::IsKeyDown(Core::Key::S)) {
			moveDir.z -= 1.0f;
		}

		if (Core::Input::IsKeyDown(Core::Key::D)) {
			moveDir.x += 1.0f;
		}

		if (glm::length(moveDir) > 0.0f) {
			moveDir = glm::normalize(moveDir);
		}

		Core::Vec3 forward = m_Transform->Forward();
		forward.y = 0.0f;
		forward = glm::normalize(forward);

		Core::Vec3 right = m_Transform->Right();
		right.y = 0.0f;
		right = glm::normalize(right);

		auto bodyID = m_PhysicsComponent->bodyID;


		Core::Vec3 position = m_Transform->Position();
		float playerHalfHeight = 1.0f; // Temporary

		Core::Vec3 rayOrigin = position;
		rayOrigin.y -= playerHalfHeight;

		float rayLength = .15f;
		
		JPH::Vec3 hitNormal;
		m_IsGrounded = Core::PhysicsManager::Raycast(JPH::Vec3{ rayOrigin.x, rayOrigin.y, rayOrigin.z }, JPH::Vec3{ 0.0f, -1.0f, 0.0f }, rayLength, bodyID, hitNormal);
		if (m_IsGrounded) {
			m_GroundNormal = Core::Vec3{ hitNormal.GetX(), hitNormal.GetY(), hitNormal.GetZ() };
		}
		
		Core::Vec3 desiredMove = ((forward * moveDir.z) + (right * moveDir.x));
		if (m_IsGrounded) {
			desiredMove = desiredMove - glm::dot(desiredMove, m_GroundNormal) * m_GroundNormal;
		}

		if (glm::length(desiredMove) > 0.0f) {
			desiredMove = glm::normalize(desiredMove);
		}

		Core::Vec3 desiredVelocity = desiredMove * m_Speed;

		JPH::Vec3 currentVelocity = Core::PhysicsManager::GetLinearVelocity(bodyID);
		Core::Vec3 currentHorizontal{ currentVelocity.GetX(), 0.0f, currentVelocity.GetZ() };

		float accel = m_IsGrounded ? m_GroundAcceleration : m_AirAcceleration;
		Core::Vec3 delta = desiredVelocity - currentHorizontal;
		Core::Vec3 acceleration = delta * accel;
		Core::Vec3 newHorizontal = currentHorizontal + acceleration * dt;

		if (glm::length(newHorizontal) > m_Speed) {
			newHorizontal = glm::normalize(newHorizontal) * m_Speed;
		}

		Core::PhysicsManager::SetLinearVelocity(bodyID, Core::Vec3{newHorizontal.x, currentVelocity.GetY(), newHorizontal.z});

		bool jumped = Core::Input::IsKeyDown(Core::Key::Space);

		if (jumped && !m_JumpedLastFrame && m_IsGrounded) {
			JPH::Vec3 currentVel = Core::PhysicsManager::GetLinearVelocity(bodyID);
			Core::Vec3 targetVel = Core::Vec3{ currentVel.GetX(), 8.0f, currentVel.GetZ() };

			Core::PhysicsManager::SetLinearVelocity(bodyID, targetVel);
		}

		m_JumpedLastFrame = jumped;
	}
}