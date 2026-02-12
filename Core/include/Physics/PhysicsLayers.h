#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

namespace Core::BroadPhaseLayers {
	static constexpr JPH::BroadPhaseLayer STATIC{ 0 };
	static constexpr JPH::BroadPhaseLayer DYNAMIC{ 1 };
	static constexpr JPH::uint NUM_LAYERS = 2;
}

namespace Core::ObjectLayers {
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::uint NUM_LAYERS = 2;
}