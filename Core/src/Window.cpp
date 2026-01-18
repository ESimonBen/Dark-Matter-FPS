#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Platform/Platform.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"


namespace Core {
	Window::Window(int width, int height, const char* title) 
		: m_Width(width), m_Height(height) {

		// Create the GLFW Window with these OpenGL version hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Forward Compatibility for MacoS
		if (Platform::IsMacOS()) {
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		}

		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);

		if (!m_Window) {
			throw std::runtime_error("Failed to create window");
		}

		glfwMakeContextCurrent(m_Window);

		// Enable VSync if it's toggled on (on by default)
		glfwSwapInterval((m_VSync ? 1 : 0));

		// https://community.khronos.org/t/glewexperimental/72738/2
		// Allows extension entry points to be loaded even if the extension isn’t present in the driver’s extensions string
		glewExperimental = GL_TRUE;

		glfwSetWindowUserPointer(m_Window, this);

		// Move glewInit into Renderer or something separate from Window
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("Failed to initialize GLEW");
		}

		// Make sure the cursor is disabled when the window is created, and support raw mouse input
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}

		glfwSetKeyCallback(m_Window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
			window->OnKeyEvent(key, scancode, action, mods);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* win, double x, double y) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
			window->OnMouseMove(x, y);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* win, double xOffset, double yOffset) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
			window->OnMouseScroll(xOffset, yOffset);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* win, int button, int action, int mods){
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
			window->OnMouseButtonEvent(button, action, mods);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* win) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
			window->OnWindowClose();
		});

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* win, int width, int height) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
			window->OnWindowResize(width, height);
		});

		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* win, int focused) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));

			if (focused == GLFW_TRUE) {
				window->OnWindowFocus();
			}
			else {
				window->OnWindowLostFocus();
			}
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* win, int x, int y) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
			window->OnWindowMove(x, y);
		});
	}

	Window::~Window() {
		if (m_Window) {
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}
	}

	bool Window::ShouldClose() const {
		return glfwWindowShouldClose(m_Window);
	}

	void Window::SwapBuffers() const {
		glfwSwapBuffers(m_Window);
	}

	void Window::PollEvents() const {
		glfwPollEvents();
	}

	int Window::GetWidth() const {
		return m_Width;
	}

	int Window::GetHeight() const {
		return m_Height;
	}

	void Window::SetEventCallback(EventFunc func) {
		m_Callback = std::move(func);
	}

	void Window::ToggleVSync() {
		m_VSync = !m_VSync;
		glfwSwapInterval((m_VSync ? 1 : 0));
	}

	void Window::OnKeyEvent(int key, int scancode, int action, int mods) {
		if (!m_Callback) {
			return;
		}

		switch (action) {
		case GLFW_PRESS:
			{
				KeyPressEvent event(key, scancode, false);
				m_Callback(event);
				break;
			}
		case GLFW_RELEASE:
			{
				KeyReleaseEvent keyRelease(key, scancode);
				m_Callback(keyRelease);
				break;
			}
		case GLFW_REPEAT:
			{
				KeyPressEvent keyRepeat(key, scancode, true);
				m_Callback(keyRepeat);
				break;
			}
		}
	}

	void Window::OnMouseMove(double xPos, double yPos) {
		if (m_Callback) {
			MouseMoveEvent mouse{ static_cast<float>(xPos), static_cast<float>(yPos) };
			m_Callback(mouse);
		}
	}

	void Window::OnMouseScroll(double xOffset, double yOffset) {
		if (m_Callback) {
			MouseScrollEvent scroll{ static_cast<float>(xOffset), static_cast<float>(yOffset) };
			m_Callback(scroll);
		}
	}

	void Window::OnMouseButtonEvent(int button, int action, int mods) {
		if (!m_Callback) {
			return;
		}

		switch (action) {
		case GLFW_PRESS:
			{
				MouseButtonPressEvent press{ button };
				m_Callback(press);
				break;
			}
		case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent release{ button };
				m_Callback(release);
				break;
			}
		}
	}

	void Window::OnWindowClose() {
		if (m_Callback) {
			WindowCloseEvent close;
			m_Callback(close);
		}
	}

	void Window::OnWindowResize(int width, int height) {
		if (m_Callback) {
			WindowResizeEvent resize{ static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };
			m_Callback(resize);
		}
	}

	void Window::OnWindowFocus() {
		if (m_Callback) {
			WindowFocusEvent focus;
			m_Callback(focus);
		}
	}

	void Window::OnWindowLostFocus() {
		if (m_Callback) {
			WindowLostFocusEvent lostFocus;
			m_Callback(lostFocus);
		}
	}

	void Window::OnWindowMove(int xPos, int yPos) {
		if (m_Callback) {
			WindowMoveEvent move(xPos, yPos);
			m_Callback(move);
		}
	}

	GLFWwindow* Window::GetNativeWindow() const {
		return m_Window;
	}

	void Window::SetCursorMode(CursorMode mode) {
		m_Cursor = mode;

		if (m_Cursor == CursorMode::Disabled) {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (glfwRawMouseMotionSupported()) {
				glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			}
		}
		else if (m_Cursor == CursorMode::Normal) {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		}
	}

	CursorMode Window::GetCursorMode() const {
		return m_Cursor;
	}
}