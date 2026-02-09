#pragma once
#include "ScriptableEntity.h"
#include <functional>
#include <memory>

namespace Core {
	struct ScriptComponent {
		std::unique_ptr<ScriptableEntity> m_Instance = nullptr;
		std::function<std::unique_ptr<ScriptableEntity>()> Instantiate;
		bool m_Initialized = false;
	};
}