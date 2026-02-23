#include "Physics/PhysicsManager.h"
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Core/Factory.h>

namespace Core {
	JPH::PhysicsSystem PhysicsManager::s_PhysicsSystem;
	JPH::TempAllocatorImpl* PhysicsManager::s_TempAllocator = nullptr;
	JPH::JobSystemThreadPool* PhysicsManager::s_JobSystem = nullptr;
	BroadPhaseLayerImplTable* PhysicsManager::s_BroadPhase = nullptr;
	ObjectLayerPairImplTable* PhysicsManager::s_ObjectPair = nullptr;
	ObjectVsBroadPhaseLayerImplTable* PhysicsManager::s_ObjectVsBroadPhase = nullptr;

	void PhysicsManager::Init() {

		const JPH::uint maxBodies = 1024;
		const JPH::uint numBodyMutexes = 0;
		const JPH::uint maxBodyPairs = 1024;
		const JPH::uint maxContactConstraints = 1024;
		const uint32_t numCores = std::max(1u, std::thread::hardware_concurrency() - 1);
		
		JPH::RegisterDefaultAllocator();
		JPH::Factory::sInstance = new JPH::Factory();
		JPH::RegisterTypes();

		s_BroadPhase = new BroadPhaseLayerImplTable();
		s_ObjectPair = new ObjectLayerPairImplTable();
		s_ObjectVsBroadPhase = new ObjectVsBroadPhaseLayerImplTable(*s_BroadPhase, *s_ObjectPair);

		s_TempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
		s_JobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, numCores);

		s_PhysicsSystem.Init(maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints, *s_BroadPhase, *s_ObjectVsBroadPhase, *s_ObjectPair);

		SetGravity({ 0.0f, -9.8f, 0.0f });
	}

	void PhysicsManager::Shutdown() {
		delete s_BroadPhase;
		delete s_ObjectPair;
		delete s_ObjectVsBroadPhase;
		delete s_TempAllocator;
		delete s_JobSystem;

		JPH::UnregisterTypes();

		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;
	}

	void PhysicsManager::Update(float dt) {
		s_PhysicsSystem.Update(dt, collisionSteps, s_TempAllocator, s_JobSystem);
	}

	JPH::BodyID PhysicsManager::CreateBox(const JPH::RVec3Arg& position, const JPH::QuatArg& rotation, const JPH::Vec3& halfExtent, bool isStatic, bool activate) {
		auto& bodyInterface = GetBodyInterface();

		JPH::RefConst<JPH::Shape> shape = new JPH::BoxShape(halfExtent);

		JPH::BodyCreationSettings settings{ shape.GetPtr(), position, rotation, isStatic ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic, isStatic ? ObjectLayers::NON_MOVING : ObjectLayers::MOVING};
		settings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
		settings.mMassPropertiesOverride.mMass = 70.0f;

		JPH::BodyID bodyID = bodyInterface.CreateAndAddBody(settings, activate ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);

		return bodyID;
	}

	void PhysicsManager::DestroyBody(JPH::BodyID id) {
		auto& bodyInterface = s_PhysicsSystem.GetBodyInterface();
		bodyInterface.RemoveBody(id);
		bodyInterface.DestroyBody(id);
	}

	JPH::BodyInterface& PhysicsManager::GetBodyInterface() {
		return s_PhysicsSystem.GetBodyInterface();
	}

	JPH::RVec3 PhysicsManager::GetPosition(JPH::BodyID id) {
		auto& bodyInterface = GetBodyInterface();
		return bodyInterface.GetPosition(id);
	}

	JPH::Quat PhysicsManager::GetRotation(JPH::BodyID id) {
		return GetBodyInterface().GetRotation(id);
	}

	JPH::PhysicsSystem& PhysicsManager::GetSystem() {
		return s_PhysicsSystem;
	}
	void PhysicsManager::SetGravity(const JPH::Vec3& gravity) {
		s_PhysicsSystem.SetGravity(gravity);
	}

	void PhysicsManager::SetRotation(JPH::BodyID id, const Quat& rotation) {
		auto& bodyInterface = GetBodyInterface();
		Quat newRotate = glm::normalize(rotation);
		bodyInterface.SetRotation(id, JPH::Quat{ newRotate.x, newRotate.y, newRotate.z, newRotate.w }, JPH::EActivation::Activate);
	}

	void PhysicsManager::SetLinearVelocity(JPH::BodyID id, const Vec3& velocity) {
		auto& bodyInterface = GetBodyInterface();
		/*Vec3 newVelocity = glm::normalize(velocity);*/
		bodyInterface.SetLinearVelocity(id, JPH::Vec3Arg{ velocity.x, velocity.y, velocity.z });
	}

	void PhysicsManager::AddForce(JPH::BodyID id, const Vec3& force) {
		auto& bodyInterface = GetBodyInterface();
		/*Vec3 newForce = glm::normalize(force);*/
		bodyInterface.AddForce(id, JPH::Vec3Arg{ force.x, force.y, force.z });
	}

	void PhysicsManager::AddImpulse(JPH::BodyID id, const Vec3& impulse) {
		auto& bodyInterface = GetBodyInterface();
		/*Vec3 newImpulse = glm::normalize(impulse);*/
		bodyInterface.AddImpulse(id, JPH::Vec3Arg{ impulse.x, impulse.y, impulse.z });
		bodyInterface.ActivateBody(id);
	}

	JPH::Vec3 PhysicsManager::GetLinearVelocity(JPH::BodyID id) {
		auto& bodyInterface = GetBodyInterface();
		return bodyInterface.GetLinearVelocity(id);
	}
}