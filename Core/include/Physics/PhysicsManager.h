#pragma once
#include <memory>
#include <Math/Types.h>
#include <Physics/BroadPhaseLayers.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Physics/ObjectLayers.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>

namespace Core {
	class PhysicsManager {
	public:
		static void Init();
		static void Shutdown();
		static void Update(float dt);
		static JPH::BodyID CreateBox(const JPH::RVec3Arg& position, const JPH::QuatArg& rotation, const JPH::Vec3& halfExtent, bool isStatic, bool activate = true);
		static JPH::BodyID CreateRamp(const JPH::RVec3Arg& position, const JPH::QuatArg& rotation, const JPH::Vec3& scale, bool isStatic, bool activate = true);
		static JPH::BodyID CreateCharacterBox(const JPH::RVec3Arg& position, const JPH::QuatArg& rotation, const JPH::Vec3& halfExtent, bool activate = true);
		static void DestroyBody(JPH::BodyID id);
		static JPH::BodyInterface& GetBodyInterface();
		static JPH::RVec3 GetPosition(JPH::BodyID id);
		static JPH::Quat GetRotation(JPH::BodyID id);
		static JPH::PhysicsSystem& GetSystem();
		static void SetGravity(const JPH::Vec3& gravity);
		static void SetRotation(JPH::BodyID id, const Quat& rotation);
		static void SetLinearVelocity(JPH::BodyID, const Vec3& velocity);
		static void AddForce(JPH::BodyID id, const Vec3& force);
		static void AddImpulse(JPH::BodyID id, const Vec3& impulse);
		static JPH::Vec3 GetLinearVelocity(JPH::BodyID id);
		static bool Raycast(const JPH::Vec3& origin, const JPH::Vec3& direction, float length, JPH::BodyID ignoredBody, JPH::Vec3& outNormal);

	private:
		static JPH::PhysicsSystem s_PhysicsSystem;
		static JPH::TempAllocatorImpl* s_TempAllocator;
		static JPH::JobSystemThreadPool* s_JobSystem;
		static BroadPhaseLayerImplTable* s_BroadPhase;
		static ObjectVsBroadPhaseLayerImplTable* s_ObjectVsBroadPhase;
		static ObjectLayerPairImplTable* s_ObjectPair;
		static constexpr int collisionSteps = 1;
	};
}