#pragma once
#include <Scene/Scene.h>

namespace Core {
	class ScriptSystem {
	public:
		static void OnCreateEntity(Scene& scene, EntityID id);
		static void OnUpdate(Scene& scene, float dt);
		static void OnFixedUpdate(Scene& scene, float dt);
		static void OnEvent(Scene& scene, Event& event);
		static void OnDestroyEntity(Scene& scene, EntityID id);
	};
}