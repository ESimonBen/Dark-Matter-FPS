#pragma once
#include "Scene/Camera.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "Scripting/ScriptableEntity.h"

namespace Core {

	struct MouseMovement {
		float deltaX = 0.0f;
		float deltaY = 0.0f;
		float lastX = 0.0f;
		float lastY = 0.0f;
		bool first = true;
	};

	class CameraControllerScript : public ScriptableEntity {
	public:
		void OnCreate() override;
		void OnUpdate(float dt) override;
		void OnEvent(Event& event) override;

	private:
		bool OnKeyPressEvent(KeyPressEvent& pressEvent);
		bool OnKeyReleaseEvent(KeyReleaseEvent& releaseEvent);
		bool OnMouseMoveEvent(MouseMoveEvent& mouseEvent);

		MouseMovement m_Mouse;
		float m_Speed = 5.0f;
		float m_Sensitivity = 0.005f;
		float m_Pitch = 0.0f;
		float m_MaxPitch = glm::radians(89.5f);
	};
}