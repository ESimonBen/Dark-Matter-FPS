#include <Physics/BroadPhaseLayers.h>

namespace Core {
	BroadPhaseLayerImplTable::BroadPhaseLayerImplTable()
		: JPH::BroadPhaseLayerInterfaceTable(ObjectLayers::NUM_LAYERS, BroadPhaseLayers::NUM_LAYERS) {
		MapObjectToBroadPhaseLayer(ObjectLayers::NON_MOVING, BroadPhaseLayers::STATIC);
		MapObjectToBroadPhaseLayer(ObjectLayers::MOVING, BroadPhaseLayers::DYNAMIC);
	}

	JPH::uint BroadPhaseLayerImplTable::GetNumBroadPhaseLayers() const {
		return BroadPhaseLayers::NUM_LAYERS;
	}

	JPH::BroadPhaseLayer BroadPhaseLayerImplTable::GetBroadPhaseLayer(JPH::ObjectLayer layer) const {
		switch (layer) {
		case ObjectLayers::NON_MOVING:
			return BroadPhaseLayers::STATIC;
		case ObjectLayers::MOVING:
			return BroadPhaseLayers::DYNAMIC;
		default:
			JPH_ASSERT(false);
			return BroadPhaseLayers::STATIC;
		}
	}
}