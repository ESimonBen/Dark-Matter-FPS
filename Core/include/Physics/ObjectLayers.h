#pragma once
#include <Physics/PhysicsLayers.h>
#include <Jolt/Physics/Collision/ObjectLayerPairFilterTable.h>
#include <Jolt/Physics/Collision/BroadPhase/ObjectVsBroadPhaseLayerFilterTable.h>

namespace Core {
	class ObjectLayerPairImplTable final : public JPH::ObjectLayerPairFilterTable {
	public:
		ObjectLayerPairImplTable();
	};

	class ObjectVsBroadPhaseLayerImplTable final : public JPH::ObjectVsBroadPhaseLayerFilterTable {
	public:
		ObjectVsBroadPhaseLayerImplTable(const JPH::BroadPhaseLayerInterface& broadPhase, const JPH::ObjectLayerPairFilter& pairFilter);
	};
}