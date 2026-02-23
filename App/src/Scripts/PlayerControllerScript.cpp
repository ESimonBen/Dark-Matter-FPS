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
		
		Core::Vec3 velocity = ((forward * moveDir.z) + (right * moveDir.x)) * m_Speed;
		JPH::Vec3 currentVelocity = Core::PhysicsManager::GetLinearVelocity(bodyID);
		velocity.y = currentVelocity.GetY();

		Core::PhysicsManager::SetLinearVelocity(bodyID, velocity);

		if (Core::Input::IsKeyDown(Core::Key::Space)) {
			Core::PhysicsManager::AddImpulse(bodyID, Core::Vec3{ 0.0f, 200.0f, 0.0f });
			auto vel = Core::PhysicsManager::GetLinearVelocity(bodyID);
			std::cout << vel.GetX() << ", "
				<< vel.GetY() << ", "
				<< vel.GetZ() << std::endl;
		}
	}
}