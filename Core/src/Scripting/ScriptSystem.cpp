#include <Scripting/ScriptSystem.h>
#include <Scripting/ScriptableEntity.h>

namespace Core {
	void ScriptSystem::OnCreateEntity(Scene& scene, EntityID id) {
		auto& sceneScripts = scene.Scripts();
		auto it = sceneScripts.find(id);

		if (it == sceneScripts.end()) {
			return;
		}

		for (auto& script : it->second) {
			if (!script.m_Initialized) {
				script.m_Instance = script.Instantiate();
				script.m_Instance->m_Entity = Entity{ id, &scene };
				script.m_Instance->m_Scene = &scene;
				script.m_Instance->OnCreate();
				script.m_Initialized = true;
			}
		}
	}

	void ScriptSystem::OnUpdate(Scene& scene, float dt) {
		auto& sceneScripts = scene.Scripts();

		for (auto& [id, scripts] : sceneScripts) {
			for (auto& script : scripts) {
				script.m_Instance->OnUpdate(dt);
			}
		}
	}

	void ScriptSystem::OnEvent(Scene& scene, Event& event) {
		auto& sceneScripts = scene.Scripts();

		for (auto& [id, scripts] : sceneScripts) {
			for (auto& script : scripts) {
				if (script.m_Initialized) {
					script.m_Instance->OnEvent(event);
				}
			}
		}
	}

	void ScriptSystem::OnDestroyEntity(Scene& scene, EntityID id) {
		auto& sceneScripts = scene.Scripts();
		auto it = sceneScripts.find(id);

		if (it == sceneScripts.end()) {
			return;
		}

		for (auto& script : it->second) {
			if (script.m_Initialized) {
				script.m_Instance->OnDestroy();
			}
		}

		sceneScripts.erase(id);
	}
}