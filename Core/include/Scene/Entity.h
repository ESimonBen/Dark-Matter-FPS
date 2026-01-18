#pragma once
#include <cstdint>

namespace Core {
	class Scene;
	using EntityID = uint32_t;


	class Entity {
	public:
		Entity() = default;
		Entity(EntityID id, Scene* scene);
		EntityID GetID() const;
		bool IsValid() const;

	private:
		EntityID m_ID = 0;
		Scene* m_Scene = nullptr;
	};
}