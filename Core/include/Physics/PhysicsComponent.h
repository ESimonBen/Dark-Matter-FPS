#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

namespace Core {
	struct PhysicsComponent {
		JPH::BodyID bodyID;
		bool isStatic = false;
	};
}