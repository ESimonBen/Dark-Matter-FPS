#pragma once
#include <Scene/Scene.h>

namespace Core {
	class PhysicsSystem {
	public:
		static void OnUpdate(Scene& scene, float dt);
		static void OnDestroy(Scene& scene);
	};
}