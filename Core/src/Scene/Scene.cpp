#include "Scene/Scene.h"
#include "Scripting/ScriptableEntity.h"
#include "Debug/Assert.h"
#include "Physics/PhysicsManager.h"
#include "Scripting/ScriptSystem.h"

namespace Core {
	Scene::Scene() {
		PhysicsManager::Init();
	}

	Entity Scene::CreateEntity() {
		EntityID id = m_NextID++;
		m_Transforms.Add(id, Transform{});
		return Entity{ id, this };
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
		PhysicsManager::Update(dt);

		for (size_t i = 0; i < m_PhysicsComponents.Size(); i++) {
			EntityID id = m_PhysicsComponents.Entities().at(i);
			auto& phys = m_PhysicsComponents.Components().at(i);

			Transform& transform = m_Transforms.Get(id);
			JPH::Vec3 pos = PhysicsManager::GetPosition(phys.bodyID);
			transform.position = { pos.GetX(), pos.GetY(), pos.GetZ() };

			JPH::Quat rotate = PhysicsManager::GetRotation(phys.bodyID);
			transform.rotation = Quat{ rotate.GetW(), rotate.GetX(), rotate.GetY(), rotate.GetZ() };
		}
	}

	void Scene::OnEvent(Event& event) {
		ScriptSystem::OnEvent(*this, event);
	}

	void Scene::OnRender(Renderer& renderer) {
		Camera& camera = GetActiveCamera();

		renderer.BeginScene(camera);

		for (size_t i = 0; i < m_MeshComponents.Size(); i++) {
			EntityID id = m_MeshComponents.Entities().at(i);
			auto& meshComponent = m_MeshComponents.Components().at(i);

			auto transform = m_Transforms.Get(id);
			renderer.Submit(meshComponent.m_Mesh, *(meshComponent.m_Program), transform.GetMatrix());
		}

		renderer.EndScene();
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

		JPH::BodyID bodyID = PhysicsManager::CreateBox(JPH::RVec3{ transform.position.x, transform.position.y, transform.position.z }, JPH::Vec3{halfExtent.x, halfExtent.y, halfExtent.z}, isStatic);

		m_PhysicsComponents.Add(entity.GetID(), {bodyID, isStatic});

		if (rotation != Quat{}) {
			auto& bodyInterface = PhysicsManager::GetBodyInterface();
			PhysicsManager::SetRotation(bodyID, rotation);
		}
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

		if (m_ActiveCamera == id) {
			m_ActiveCamera = 0;
		}

		m_Transforms.Remove(id);
		m_MeshComponents.Remove(id);
		m_Cameras.Remove(id);
		m_PhysicsComponents.Remove(id);
	}

	bool Scene::IsEntityAlive(EntityID id) const {
		return m_Transforms.Has(id);
	}
}