#include <GLFW/glfw3.h>
#include <iostream>
#include "Scripts/CameraControllerScript.h"
#include "Scene/Scene.h"
#include "Input/Input.h"
#include "Physics/PhysicsManager.h"

namespace DarkMatter {
	void CameraControllerScript::OnCreate() {
		// Nothing for now
	}

	void CameraControllerScript::OnUpdate(float dt) {
		if (!m_Scene->IsCursorLocked()) {
			m_Mouse.first = true;
			return;
		}

		Core::Transform& cameraTransform = GetCamera().GetTransform();
		auto* parent = cameraTransform.GetParent();

		if (!parent) {
			return;
		}

		Core::Entity playerEntity = Core::Entity{ parent->GetOwner(), m_Scene };
		Core::Vec2 mouse = Core::Input::GetMouseDelta();

		float yaw = -mouse.x * m_Sensitivity;
		float pitch = -mouse.y * m_Sensitivity;
		float newPitch = glm::clamp(m_Pitch + pitch, -m_MaxPitch, m_MaxPitch);
		pitch = newPitch - m_Pitch;
		m_Pitch = newPitch;

		auto& phys = m_Scene->PhysicsComponents().Get(playerEntity.GetID());

		JPH::Quat currentRotation = Core::PhysicsManager::GetRotation(phys.bodyID);
		Core::Quat yawRotation = glm::angleAxis(yaw, Core::Vec3{ 0.0f, 1.0f, 0.0f });
		Core::Quat newRotation = yawRotation * Core::Quat{ currentRotation.GetW(), currentRotation.GetX(), currentRotation.GetY(), currentRotation.GetZ() };

		Core::PhysicsManager::SetRotation(phys.bodyID, newRotation);

		/*playerTransform.RotateYaw(yaw);*/
		cameraTransform.RotatePitch(pitch);
	}

	void CameraControllerScript::OnEvent(Core::Event& event) {
		Core::EventDispatcher dispatcher{ event };

		dispatcher.TryDispatch<Core::KeyPressEvent>([this](Core::KeyPressEvent& event) {
			return OnKeyPressEvent(event);
		});

		dispatcher.TryDispatch<Core::KeyReleaseEvent>([this](Core::KeyReleaseEvent& event) {
			return OnKeyReleaseEvent(event);
		});

		dispatcher.TryDispatch<Core::MouseMoveEvent>([this](Core::MouseMoveEvent& event) {
			return OnMouseMoveEvent(event);
		});
	}

	bool CameraControllerScript::OnKeyPressEvent(Core::KeyPressEvent& pressEvent) {
		if (pressEvent.GetKeyCode() == GLFW_KEY_ESCAPE && !pressEvent.GetRepeat()) {
			m_Scene->SetCursorLocked(!m_Scene->IsCursorLocked());
			m_Mouse.first = true;
			m_Mouse.deltaX = 0.0f;
			m_Mouse.deltaY = 0.0f;
			return true;
		}

		if (!m_Scene->IsCursorLocked()) {
			return false;
		}

		switch (pressEvent.GetKeyCode()) {
		case GLFW_KEY_W:
			Core::Input::SetKey(Core::Key::W, true);
			break;
		case GLFW_KEY_A:
			Core::Input::SetKey(Core::Key::A, true);
			break;
		case GLFW_KEY_S:
			Core::Input::SetKey(Core::Key::S, true);
			break;
		case GLFW_KEY_D:
			Core::Input::SetKey(Core::Key::D, true);
			break;
		case GLFW_KEY_SPACE:
			Core::Input::SetKey(Core::Key::Space, true);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			Core::Input::SetKey(Core::Key::LeftShift, true);
			break;
		}

		return true;
	}

	bool CameraControllerScript::OnKeyReleaseEvent(Core::KeyReleaseEvent& releaseEvent) {
		switch (releaseEvent.GetKeyCode()) {
		case GLFW_KEY_W:
			Core::Input::SetKey(Core::Key::W, false);
			break;
		case GLFW_KEY_A:
			Core::Input::SetKey(Core::Key::A, false);
			break;
		case GLFW_KEY_S:
			Core::Input::SetKey(Core::Key::S, false);
			break;
		case GLFW_KEY_D:
			Core::Input::SetKey(Core::Key::D, false);
			break;
		case GLFW_KEY_SPACE:
			Core::Input::SetKey(Core::Key::Space, false);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			Core::Input::SetKey(Core::Key::LeftShift, false);
			break;
		}

		return true;
	}

	bool CameraControllerScript::OnMouseMoveEvent(Core::MouseMoveEvent& mouseEvent) {
		if (!m_Scene->IsCursorLocked()) {
			m_Mouse.first = true;
			return false;
		}

		float x = mouseEvent.GetX();
		float y = mouseEvent.GetY();

		if (m_Mouse.first) {
			m_Mouse.lastX = x;
			m_Mouse.lastY = y;
			m_Mouse.first = false;
			return true;
		}

		float dx = x - m_Mouse.lastX;
		float dy = y - m_Mouse.lastY;

		m_Mouse.lastX = x;
		m_Mouse.lastY = y;



		Core::Input::SetMouseDelta(dx, dy);

		return true;
	}

}