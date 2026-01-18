#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "Platform/Platform.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Mesh.h"
#include "Scene/Scene.h"

namespace Core {
	Application::Application() {
		// Nothing for now
	}

	Application::~Application() {
		// Nothing for now
	}

	void Application::OnInit() {
		window = std::make_unique<Window>(960, 720, "Dark Matter");
		camera = std::make_unique<Camera>(Vec3{ 0.0f, 0.0f, 3.0f }, Vec3{ 0.0f, 1.0f, 0.0f }, glm::pi<float>() / 4.0f, window->GetWidth(), window->GetHeight(), 0.1f, 100.0f);
		controller = std::make_unique<CameraController>(*camera, *window);
		scene = std::make_unique<Scene>();
		renderer = std::make_unique<Renderer>();
	}

	void Application::OnEvent(Event& event) {
		controller->OnEvent(event);
	}

	void Application::OnUpdate(float dt) {
		controller->OnUpdate(dt);
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
			scene->OnRender(*renderer, *camera);

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
		ShaderProgram shader_program;
		shader_program.Attach(vert);
		shader_program.Attach(frag);
		shader_program.Link();

		Entity cube = scene->CreateEntity();
		scene->AttachMesh(cube, std::make_unique<Mesh>(std::move(mesh)), std::make_unique<ShaderProgram>(std::move(shader_program)));
	}
}