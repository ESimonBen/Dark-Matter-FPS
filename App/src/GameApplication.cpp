#include "GameApplication.h"
#include "Scripts/CameraControllerScript.h"
#include "Rendering/ShaderProgram.h"

namespace DarkMatter {
	void GameApplication::OnInit() {
		Core::Application::OnInit();

		auto& scene = GetScene();
		auto& window = GetWindow();

		Core::Entity cameraEntity = scene.CreateEntity();

		auto& camera = scene.CreateCamera(cameraEntity, glm::pi<float>() / 4.0f, window.GetWidth(), window.GetHeight(), 0.1f, 100.0f);

		cameraEntity.GetTransform().SetPosition({ 0.0f, -3.0f, 0.0f });

		scene.AttachScript<CameraControllerScript>(cameraEntity);
		scene.SetActiveCamera(cameraEntity);

		LoadTestScene();

	}
	
	void GameApplication::LoadTestScene() {
		auto& scene = GetScene();

		Core::Shader vert{ "assets/shaders/vertex.glsl", Core::ShaderType::Vertex };
		Core::Shader frag{ "assets/shaders/fragment.glsl", Core::ShaderType::Fragment };

		auto shaderProgram = std::make_shared<Core::ShaderProgram>();
		shaderProgram->Attach(vert);
		shaderProgram->Attach(frag);
		shaderProgram->Link();

		Core::Entity floor = scene.CreateEntity();
		floor.GetTransform().SetPosition({ 0.0f, -5.0f, 0.0f });
		floor.GetTransform().SetScale({ 40.0f, 2.0f, 40.0f });
		scene.AttachPhysicsBox(floor, { 20.0f, 1.0f, 20.0f }, true, { 1.0f, 0.0f, 0.0f, 0.0f });
		scene.AttachMesh(floor, std::move(CreateCubeMesh()), shaderProgram);

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				Core::Entity cube1 = scene.CreateEntity();
				cube1.GetTransform().SetPosition({ (float)i, (float)j + 10.0f, -10.0f });
				scene.AttachPhysicsBox(cube1, { 0.5f, 0.5f, 0.5f }, false, { 1.0f, 0.0f, 0.0f, 0.0f });
				scene.AttachMesh(cube1, std::move(CreateCubeMesh()), shaderProgram);
			}
		}
	}

	Core::Mesh GameApplication::CreateCubeMesh() {
		Core::Vertex vertices[] = {
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

		return Core::Mesh{ vertices, sizeof(vertices), indices, sizeof(indices) };
	}

}