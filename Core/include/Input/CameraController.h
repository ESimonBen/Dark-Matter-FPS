#pragma once
#include "Scene/Camera.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace Core {
	struct KeyBoardMovement {
		bool forward = false;
		bool backward = false;
		bool right = false;
		bool left = false;
		bool up = false;
		bool down = false;
	};

	struct MouseMovement {
		float deltaX = 0.0f;
		float deltaY = 0.0f;
		float lastX = 0.0f;
		float lastY = 0.0f;
		bool first = true;
	};

	class CameraController {
	public:
		CameraController(Camera& camera, Window& window);
		void OnUpdate(float dt);
		void OnEvent(Event& event);
		bool OnKeyPressEvent(KeyPressEvent& pressEvent);
		bool OnKeyReleaseEvent(KeyReleaseEvent& releaseEvent);
		bool OnMouseMoveEvent(MouseMoveEvent& mouseEvent);
		bool OnWindowResizeEvent(WindowResizeEvent& resizeEvent);

	private:
		void ToggleCursor();

		Camera& m_Camera;
		Window& m_Window;
		MouseMovement m_Mouse;
		KeyBoardMovement m_Keyboard;
		float m_Speed = 5.0f;
		float m_Sensitivity = 0.005f;
		bool m_CursorLocked = true;
	};
}