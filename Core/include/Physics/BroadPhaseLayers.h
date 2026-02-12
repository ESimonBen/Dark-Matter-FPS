#pragma once
#include <Physics/PhysicsLayers.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayerInterfaceTable.h>

namespace Core {
	class BroadPhaseLayerImplTable final : public JPH::BroadPhaseLayerInterfaceTable {
	public:
		BroadPhaseLayerImplTable();

		JPH::uint GetNumBroadPhaseLayers() const override final;

		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override final;
	};
}