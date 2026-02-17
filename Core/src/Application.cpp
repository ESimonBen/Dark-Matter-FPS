#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "Platform/Platform.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Mesh.h"
#include "Scene/Scene.h"
#include "Events/ApplicationEvent.h"
#include "Input/Input.h"

namespace Core {
	Application::Application() {
		// Nothing for now
	}

	Application::~Application() {
		// Nothing for now
	}

	void Application::OnInit() {
		window = std::make_unique<Window>(1280, 720, "Dark Matter");
		scene = std::make_unique<Scene>();
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(event);

		dispatcher.TryDispatch<WindowResizeEvent>( [this](WindowResizeEvent& e) {
				glViewport(0, 0, e.GetWidth(), e.GetHeight());
				scene->OnWindowResize(e);
				return false;
		});

		scene->OnEvent(event);
	}

	void Application::OnUpdate(float dt) {
		static bool lastLocked = scene->IsCursorLocked();

		bool locked = scene->IsCursorLocked();
		if (locked != lastLocked) {
			window->SetCursorMode(
				locked ? CursorMode::Disabled : CursorMode::Normal
			);
			lastLocked = locked;
		}

		scene->OnUpdate(dt);
	}

	void Application::OnFixedUpdate(float dt) {
		scene->OnFixedUpdate(dt);
	}

	void Application::Run() {
		Platform::Init();
		OnInit();

		window->SetEventCallback([this](Event& e) {
			OnEvent(e);
		});

		float accumulator = 0.0f;
		const float fixedDelta = 1.0f / 60.0f;

		while (!window->ShouldClose()) {
			float time = glfwGetTime();
			m_DeltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Prevents spiral of death
			m_DeltaTime = std::min(m_DeltaTime, 0.25f);
			accumulator += m_DeltaTime;

			window->PollEvents();

			while (accumulator >= fixedDelta) {
				OnFixedUpdate(fixedDelta);
				accumulator -= fixedDelta;
			}

			OnUpdate(m_DeltaTime);

			float alpha = accumulator / fixedDelta;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			scene->OnRender(renderer, alpha);

			Input::EndFrame();
			window->SwapBuffers();
		}

		OnShutdown();
	}

	void Application::OnShutdown() {
		if (window) {
			window.reset();
		}

		Platform::Shutdown();
	}

	Scene& Application::GetScene() {
		return *scene;
	}

	Window& Application::GetWindow() {
		return *window;
	}

	Renderer& Application::GetRenderer() {
		return renderer;
	}
}