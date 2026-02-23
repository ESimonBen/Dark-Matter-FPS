#include <Physics/PhysicsSystem.h>
#include <Physics/PhysicsManager.h>

namespace Core {
	void PhysicsSystem::OnUpdate(Scene& scene, float dt) {

		auto& physicsComponents = scene.PhysicsComponents();
		auto& transforms = scene.Transforms();

		for (size_t i = 0; i < transforms.Size(); i++) {
			EntityID id = transforms.Entities()[i];
			Transform& transform = transforms.Get(id);
			transform.SetPreviousPosition(transform.Position());
			transform.SetPreviousRotation(transform.Rotation());
		}

		PhysicsManager::Update(dt);

		for (size_t i = 0; i < physicsComponents.Size(); i++) {
			EntityID id = physicsComponents.Entities()[i];
			auto& phys = physicsComponents.Components()[i];

			Transform& transform = transforms.Get(id);

			JPH::Vec3 pos = PhysicsManager::GetPosition(phys.bodyID);
			transform.SetPosition({ pos.GetX(), pos.GetY(), pos.GetZ() });

			JPH::Quat rotate = PhysicsManager::GetRotation(phys.bodyID);
			transform.SetRotation({rotate.GetW(), rotate.GetX(), rotate.GetY(), rotate.GetZ()});
		}
	}

	void PhysicsSystem::OnDestroy(Scene& scene) {
		auto& physicsComponents = scene.PhysicsComponents();

		for (size_t i = physicsComponents.Size() - 1; i-- > 0;) {
			EntityID id = physicsComponents.Entities()[i];
			auto& phys = physicsComponents.Components()[i];

			PhysicsManager::DestroyBody(phys.bodyID);
			physicsComponents.Remove(id);
		}
	}
}