#pragma once
#include <functional>
#include <stdexcept>
#include "Events/Event.h"

struct GLFWwindow;

namespace Core {
	enum class CursorMode {
		Normal, Disabled
	};

	class Window {
	public:
		using EventFunc = std::function<void(Event&)>;

		Window(int width, int height, const char* title);
		~Window();

		bool ShouldClose() const;
		void SwapBuffers() const;
		void PollEvents() const;
		int GetWidth() const;
		int GetHeight() const;
		void SetEventCallback(EventFunc func);
		void ToggleVSync();
		GLFWwindow* GetNativeWindow() const;
		void SetCursorMode(CursorMode mode);
		CursorMode GetCursorMode() const;

	private:
		void OnKeyEvent(int key, int scancode, int action, int mods);
		void OnMouseMove(double xPos, double yPos);
		void OnMouseScroll(double xOffset, double yOffset);
		void OnMouseButtonEvent(int button, int action, int mods);
		void OnWindowClose();
		void OnWindowResize(int width, int height);
		void OnWindowFocus();
		void OnWindowLostFocus();
		void OnWindowMove(int xPos, int yPos);

		EventFunc m_Callback;
		GLFWwindow* m_Window = nullptr;
		CursorMode m_Cursor = CursorMode::Normal;
		int m_Width, m_Height;
		bool m_VSync = true;
	};
}