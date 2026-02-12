#include <Physics/ObjectLayers.h>

namespace Core {
	ObjectLayerPairImplTable::ObjectLayerPairImplTable()
		: JPH::ObjectLayerPairFilterTable(ObjectLayers::NUM_LAYERS) {
		EnableCollision(ObjectLayers::NON_MOVING, ObjectLayers::MOVING);
		EnableCollision(ObjectLayers::MOVING, ObjectLayers::MOVING);
	}


	ObjectVsBroadPhaseLayerImplTable::ObjectVsBroadPhaseLayerImplTable(const JPH::BroadPhaseLayerInterface& broadPhase, const JPH::ObjectLayerPairFilter& pairFilter)
		: JPH::ObjectVsBroadPhaseLayerFilterTable(broadPhase, BroadPhaseLayers::NUM_LAYERS, pairFilter, ObjectLayers::NUM_LAYERS)
	{}
}