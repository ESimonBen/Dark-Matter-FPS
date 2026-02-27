#include "Scene/Scene.h"
#include "Scripting/ScriptableEntity.h"
#include "Debug/Assert.h"
#include "Physics/PhysicsManager.h"
#include "Scripting/ScriptSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Rendering/RenderSystem.h"

namespace Core {
	Scene::Scene() {
		PhysicsManager::Init();
	}

	Entity Scene::CreateEntity() {
		EntityID id = m_NextID++;
		auto& transform = m_Transforms.Add(id, Transform{});
		transform.SetOwner(id);
		return Entity{ id, this };
	}

	void Scene::SetParent(Entity parent, Entity child) {
		parent.GetTransform().AddChild(child);
	}

	Camera& Scene::CreateCamera(Entity entity, float fov, float width, float height, float near_plane, float far_plane) {
		EntityID id = entity.GetID();
		m_Cameras.Add(id, Camera{ entity, fov, width, height, near_plane, far_plane });

		return m_Cameras.Get(id);
	}

	void Scene::SetActiveCamera(Entity entity) {
		m_ActiveCamera = entity.GetID();
	}

	Camera& Scene::GetActiveCamera() {
		return m_Cameras.Get(m_ActiveCamera);
	}

	void Scene::InitializeScripts(EntityID id) {
		ScriptSystem::OnCreateEntity(*this, id);
	}

	void Scene::OnUpdate(float dt) {
		ScriptSystem::OnUpdate(*this, dt);
	}

	void Scene::OnFixedUpdate(float dt) {
		PhysicsSystem::OnUpdate(*this, dt);
		ScriptSystem::OnFixedUpdate(*this, dt);
	}

	void Scene::OnEvent(Event& event) {
		ScriptSystem::OnEvent(*this, event);
	}

	void Scene::OnRender(Renderer& renderer, float alpha) {
		RenderSystem::OnRender(*this, renderer, alpha);
	}

	Transform& Scene::GetTransform(Entity entity) {
		return m_Transforms.Get(entity.GetID()); // Return the Transform of the Entity
	}

	Camera& Scene::GetCamera(Entity entity) {
		return m_Cameras.Get(entity.GetID());
	}

	void Scene::AttachMesh(Entity entity, Mesh mesh, std::shared_ptr<ShaderProgram> program) {
		CORE_ASSERT(entity.IsValid())
		EntityID id = entity.GetID();
		MeshComponent meshComponent{ std::move(mesh), program };
		m_MeshComponents.Add(id, std::move(meshComponent));
	}

	void Scene::AttachPhysicsBox(Entity entity, const Vec3& halfExtent, bool isStatic, Quat rotation) {
		Transform& transform = GetTransform(entity);

		if (rotation != Quat{}) {
			transform.SetRotation(rotation);
		}
		
		transform.SetPreviousPosition(transform.Position());
		transform.SetPreviousRotation(transform.Rotation());

		JPH::BodyID bodyID = PhysicsManager::CreateBox(JPH::RVec3{ transform.Position().x, transform.Position().y, transform.Position().z }, JPH::QuatArg{rotation.x, rotation.y, rotation.z, rotation.w}, 
													   JPH::Vec3{halfExtent.x, halfExtent.y, halfExtent.z}, isStatic);

		m_PhysicsComponents.Add(entity.GetID(), {bodyID, isStatic});
	}

	void Scene::AttachCharacterBox(Entity entity, const Vec3& halfExtent, Quat rotation) {
		Transform& transform = GetTransform(entity);

		if (rotation != Quat{}) {
			transform.SetRotation(rotation);
		}

		transform.SetPreviousPosition(transform.Position());
		transform.SetPreviousRotation(transform.Rotation());

		JPH::BodyID bodyID = PhysicsManager::CreateCharacterBox(JPH::RVec3{ transform.Position().x, transform.Position().y, transform.Position().z }, JPH::QuatArg{ rotation.x, rotation.y, rotation.z, rotation.w },
			JPH::Vec3{ halfExtent.x, halfExtent.y, halfExtent.z });

		m_PhysicsComponents.Add(entity.GetID(), { bodyID, false });
	}

	bool Scene::HasMesh(EntityID id) const {
		return m_MeshComponents.Has(id);
	}

	void Scene::OnWindowResize(WindowResizeEvent& resize) {
		Camera& cam = GetActiveCamera();
		cam.SetAspectRatio(resize.GetWidth(), resize.GetHeight());
	}

	void Scene::SetCursorLocked(bool mode){
		m_CursorLocked = mode;
	}

	bool Scene::IsCursorLocked() const {
		return m_CursorLocked;
	}

	bool Scene::ConsumeCursorDirty() {
		if (m_CursorDirty) {
			m_CursorDirty = false;
			return true;
		}

		return false;
	}

	void Scene::DestroyEntity(Entity entity){
		CORE_ASSERT(entity.IsValid())
		EntityID id = entity.GetID();
		ScriptSystem::OnDestroyEntity(*this, id);
		PhysicsSystem::OnDestroy(*this);

		if (m_ActiveCamera == id) {
			m_ActiveCamera = 0;
		}

		m_Transforms.Remove(id);
		m_MeshComponents.Remove(id);
		m_PhysicsComponents.Remove(id);
	}

	bool Scene::IsEntityAlive(EntityID id) const {
		return m_Transforms.Has(id);
	}

	ComponentStorage<Transform>& Scene::Transforms() {
		return m_Transforms;
	}

	ComponentStorage<PhysicsComponent>& Scene::PhysicsComponents() {
		return m_PhysicsComponents;
	}

	ComponentStorage<MeshComponent>& Scene::MeshComponents() {
		return m_MeshComponents;
	}

	std::unordered_map<EntityID, std::vector<ScriptComponent>>& Scene::Scripts() {
		return m_Scripts;
	}
}