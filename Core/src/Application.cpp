#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "Platform/Platform.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Mesh.h"
#include "Scene/Scene.h"
#include "Scripting/CameraControllerScript.h"
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
		Entity cameraEntity = scene->CreateEntity();
		Camera& camera = scene->CreateCamera(cameraEntity, glm::pi<float>() / 4.0f, window->GetWidth(), window->GetHeight(), 0.1f, 100.0f);
		scene->AttachScript<CameraControllerScript>(cameraEntity);
		scene->SetActiveCamera(cameraEntity);
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

	void Application::Run() {
		Platform::Init();
		OnInit();
		LoadTestScene();

		window->SetEventCallback([this](Event& e) {
			OnEvent(e);
		});

		while (!window->ShouldClose()) {
			float time = glfwGetTime();
			m_DeltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			window->PollEvents();
			OnUpdate(m_DeltaTime);
			scene->OnRender(renderer);
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

	void Application::LoadTestScene() {
		Vertex vertices[] = {
			// index: 0
			{ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} }, // left-bottom-back   (red)
			// index: 1
			{ { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} }, // right-bottom-back  (green)
			// index: 2
			{ { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f} }, // right-top-back    (blue)
			// index: 3
			{ {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f} }, // left-top-back     (yellow)

			// index: 4
			{ {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f} }, // left-bottom-front (magenta)
			// index: 5
			{ { 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f} }, // right-bottom-front(cyan)
			// index: 6
			{ { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f} }, // right-top-front   (white)
			// index: 7
			{ {-0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f} }, // left-top-front    (black)
		};

		unsigned int indices[] = {
			// back face (-Z)
			0, 1, 2,
			2, 3, 0,

			// front face (+Z)
			4, 6, 5,
			6, 4, 7,

			// left face (-X)
			0, 3, 7,
			7, 4, 0,

			// right face (+X)
			1, 5, 6,
			6, 2, 1,

			// bottom face (-Y)
			0, 4, 5,
			5, 1, 0,

			// top face (+Y)
			3, 2, 6,
			6, 7, 3
		};

		Shader vert{ "assets/shaders/vertex.glsl", ShaderType::Vertex };
		Shader frag{ "assets/shaders/fragment.glsl", ShaderType::Fragment };
		Mesh mesh{ vertices, sizeof(vertices), indices, sizeof(indices) };
		Mesh mesh2{ vertices, sizeof(vertices), indices, sizeof(indices) };

		auto shaderProgram = std::make_shared<ShaderProgram>();
		shaderProgram->Attach(vert);
		shaderProgram->Attach(frag);
		shaderProgram->Link();

		Entity cube1 = scene->CreateEntity();
		cube1.GetTransform().position = {1.0f, 0.0f, -3.0f};
		scene->AttachMesh(cube1, std::make_unique<Mesh>(std::move(mesh)), shaderProgram);

		Entity cube2 = scene->CreateEntity();
		cube2.GetTransform().position = {-1.0f, 0.0f, -3.0f};
		scene->AttachMesh(cube2, std::make_unique<Mesh>(std::move(mesh2)), shaderProgram);
	}
}