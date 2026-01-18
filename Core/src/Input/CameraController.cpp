#include <GLFW/glfw3.h>
#include <iostream>
#include "Input/CameraController.h"

namespace Core {
	CameraController::CameraController(Camera& camera, Window& window) 
		: m_Camera(camera), m_Window(window)
	{}

	void CameraController::OnUpdate(float dt) {
		if (!m_CursorLocked) {
			return;
		}

		if (m_Keyboard.forward == true) {
			m_Camera.Translate(m_Camera.Front() * m_Speed, dt);
		}
		if (m_Keyboard.backward == true) {
			m_Camera.Translate(-m_Camera.Front() * m_Speed, dt);
		}
		if (m_Keyboard.right == true) {
			m_Camera.Translate(m_Camera.Right() * m_Speed, dt);
		}
		if (m_Keyboard.left == true) {
			m_Camera.Translate(-m_Camera.Right() * m_Speed, dt);
		}
		if (m_Keyboard.up == true) {
			m_Camera.Translate(Vec3{ 0.0f, 1.0f, 0.0f } * m_Speed, dt);
		}
		if (m_Keyboard.down == true) {
			m_Camera.Translate(Vec3{ 0.0f, -1.0f, 0.0f } * m_Speed, dt);
		}

		m_Camera.AdjustYaw(m_Mouse.deltaX * m_Sensitivity);
		m_Camera.AdjustPitch(-m_Mouse.deltaY * m_Sensitivity);
		m_Mouse.deltaX = 0.0f;
		m_Mouse.deltaY = 0.0f;
	}

	void CameraController::OnEvent(Event& event) {
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

		dispatcher.TryDispatch<WindowResizeEvent>([this](WindowResizeEvent& event) {
			return OnWindowResizeEvent(event);
		});
	}

	bool CameraController::OnKeyPressEvent(KeyPressEvent& pressEvent) {
		switch (pressEvent.GetKeyCode()) {
		case GLFW_KEY_W:
			m_Keyboard.forward = true;
			break;
		case GLFW_KEY_A:
			m_Keyboard.left = true;
			break;
		case GLFW_KEY_S:
			m_Keyboard.backward = true;
			break;
		case GLFW_KEY_D:
			m_Keyboard.right = true;
			break;
		case GLFW_KEY_SPACE:
			m_Keyboard.up = true;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			m_Keyboard.down = true;
			break;
		case GLFW_KEY_ESCAPE:
			ToggleCursor();
			break;
		}

		return true;
	}

	bool CameraController::OnKeyReleaseEvent(KeyReleaseEvent& releaseEvent) {
		switch (releaseEvent.GetKeyCode()) {
		case GLFW_KEY_W:
			m_Keyboard.forward = false;
			break;
		case GLFW_KEY_A:
			m_Keyboard.left = false;
			break;
		case GLFW_KEY_S:
			m_Keyboard.backward = false;
			break;
		case GLFW_KEY_D:
			m_Keyboard.right = false;
			break;
		case GLFW_KEY_SPACE:
			m_Keyboard.up = false;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			m_Keyboard.down = false;
			break;
		}

		return true;
	}

	bool CameraController::OnMouseMoveEvent(MouseMoveEvent& mouseEvent) {
		if (m_Mouse.first) {
			m_Mouse.lastX = mouseEvent.GetX();
			m_Mouse.lastY = mouseEvent.GetY();
			m_Mouse.first = false;
			return true;
		}

		m_Mouse.deltaX += (mouseEvent.GetX() - m_Mouse.lastX);
		m_Mouse.deltaY += (mouseEvent.GetY() - m_Mouse.lastY);
		m_Mouse.lastX = mouseEvent.GetX();
		m_Mouse.lastY = mouseEvent.GetY();
		return true;
	}

	bool CameraController::OnWindowResizeEvent(WindowResizeEvent& resizeEvent) {
		glViewport(0, 0, resizeEvent.GetWidth(), resizeEvent.GetHeight());
		m_Camera.SetAspectRatio(resizeEvent.GetWidth(), resizeEvent.GetHeight());
		return true;
	}

	void CameraController::ToggleCursor() {
		m_CursorLocked = !m_CursorLocked;

		m_Window.SetCursorMode(
			m_CursorLocked ? CursorMode::Disabled : CursorMode::Normal
		);

		m_Mouse.first = true;
		m_Mouse.deltaX = m_Mouse.deltaY = 0.0f;
	}
}