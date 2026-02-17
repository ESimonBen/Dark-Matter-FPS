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
		cameraEntity.GetTransform().SetPosition({ 0.0f, -3.0f, 0.0f });
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
				OnUpdate(fixedDelta);
				accumulator -= fixedDelta;
			}

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

	void Application::LoadTestScene() {
		Shader vert{ "assets/shaders/vertex.glsl", ShaderType::Vertex };
		Shader frag{ "assets/shaders/fragment.glsl", ShaderType::Fragment };

		auto shaderProgram = std::make_shared<ShaderProgram>();
		shaderProgram->Attach(vert);
		shaderProgram->Attach(frag);
		shaderProgram->Link();

		Entity floor = scene->CreateEntity();
		floor.GetTransform().SetPosition({0.0f, -5.0f, 0.0f});
		floor.GetTransform().SetScale({ 40.0f, 2.0f, 40.0f });
		scene->AttachPhysicsBox(floor, { 20.0f, 1.0f, 20.0f }, true, {1.0f, 0.0f, 0.0f, 0.0f});
		scene->AttachMesh(floor, std::move(CreateCubeMesh()), shaderProgram);

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				Entity cube1 = scene->CreateEntity();
				cube1.GetTransform().SetPosition({ (float)i, (float)j + 10.0f, -10.0f });
				scene->AttachPhysicsBox(cube1, { 0.5f, 0.5f, 0.5f }, false, { 1.0f, 0.0f, 0.0f, 0.0f });
				scene->AttachMesh(cube1, std::move(CreateCubeMesh()), shaderProgram);
			}
		}
	}

	Mesh Application::CreateCubeMesh() {
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

		return Mesh{ vertices, sizeof(vertices), indices, sizeof(indices) };
	}
}