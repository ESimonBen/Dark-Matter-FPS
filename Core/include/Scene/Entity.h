#pragma once
#include "Transform.h"
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
		Scene* GetScene() const;

		Transform& GetTransform();
		bool HasMesh() const;

		bool operator==(const Entity& other) const;
		bool operator!=(const Entity& other) const;

	private:
		Scene* m_Scene = nullptr;
		EntityID m_ID = 0;
	};
}