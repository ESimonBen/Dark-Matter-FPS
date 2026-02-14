#include <GLFW/glfw3.h>
#include <iostream>
#include "Scripting/CameraControllerScript.h"
#include "Scene/Scene.h"
#include "Input/Input.h"

namespace Core {
	void CameraControllerScript::OnCreate() {
		// Nothing for now
	}

	void CameraControllerScript::OnUpdate(float dt) {
		if (!m_Scene->IsCursorLocked()) {
			m_Mouse.first = true;
			return;
		}

		Transform& t = GetCamera().GetTransform();
		Vec2 mouse = Input::GetMouseDelta();

		float yaw = -mouse.x * m_Sensitivity;
		float pitch = -mouse.y * m_Sensitivity;
		float newPitch = glm::clamp(m_Pitch + pitch, -m_MaxPitch, m_MaxPitch);
		pitch = newPitch - m_Pitch;
		m_Pitch = newPitch;

		t.RotateYaw(yaw);
		t.RotatePitch(pitch);

		m_Mouse.deltaX = 0.0f;
		m_Mouse.deltaY = 0.0f;

		Vec3 moveDir{ 0.0f };

		if (Input::IsKeyDown(Key::W))	      moveDir.z += 1.0f;
		if (Input::IsKeyDown(Key::A))	      moveDir.x -= 1.0f;
		if (Input::IsKeyDown(Key::S))	      moveDir.z -= 1.0f;
		if (Input::IsKeyDown(Key::D))	      moveDir.x += 1.0f;
		if (Input::IsKeyDown(Key::Space))     moveDir.y += 1.0f;
		if (Input::IsKeyDown(Key::LeftShift)) moveDir.y -= 1.0f;

		if (glm::length(moveDir) > 0.0f) {
			moveDir = glm::normalize(moveDir);
		}
		
		t.TranslateLocal(Vec3{ moveDir.x, 0.0f, moveDir.z } * m_Speed * dt);
		t.TranslateWorld(Vec3{ 0.0f, moveDir.y, 0.0f } * m_Speed * dt);
	}

	void CameraControllerScript::OnEvent(Event& event) {
		EventDispatcher dispatcher{ event };

		dispatcher.TryDispatch<KeyPressEvent>([this](KeyPressEvent& event) {
			return OnKeyPressEvent(event);
		});

		dispatcher.TryDispatch<KeyReleaseEvent>([this](KeyReleaseEvent& event) {
			return OnKeyReleaseEvent(event);
		});

		dispatcher.TryDispatch<MouseMoveEvent>([this](MouseMoveEvent& event) {
			return OnMouseMoveEvent(event);
		});
	}

	bool CameraControllerScript::OnKeyPressEvent(KeyPressEvent& pressEvent) {
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
			Input::SetKey(Key::W, true);
			break;
		case GLFW_KEY_A:
			Input::SetKey(Key::A, true);
			break;
		case GLFW_KEY_S:
			Input::SetKey(Key::S, true);
			break;
		case GLFW_KEY_D:
			Input::SetKey(Key::D, true);
			break;
		case GLFW_KEY_SPACE:
			Input::SetKey(Key::Space, true);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			Input::SetKey(Key::LeftShift, true);
			break;
		}

		return true;
	}

	bool CameraControllerScript::OnKeyReleaseEvent(KeyReleaseEvent& releaseEvent) {
		switch (releaseEvent.GetKeyCode()) {
		case GLFW_KEY_W:
			Input::SetKey(Key::W, false);
			break;
		case GLFW_KEY_A:
			Input::SetKey(Key::A, false);
			break;
		case GLFW_KEY_S:
			Input::SetKey(Key::S, false);
			break;
		case GLFW_KEY_D:
			Input::SetKey(Key::D, false);
			break;
		case GLFW_KEY_SPACE:
			Input::SetKey(Key::Space, false);
			break;
		case GLFW_KEY_LEFT_SHIFT:
			Input::SetKey(Key::LeftShift, false);
			break;
		}

		return true;
	}

	bool CameraControllerScript::OnMouseMoveEvent(MouseMoveEvent& mouseEvent) {
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



		Input::SetMouseDelta(dx, dy);

		return true;
	}

}