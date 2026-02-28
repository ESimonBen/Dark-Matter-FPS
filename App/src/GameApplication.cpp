#include "GameApplication.h"
#include "Scripts/CameraControllerScript.h"
#include "Scripts/PlayerControllerScript.h"
#include "Rendering/ShaderProgram.h"

namespace DarkMatter {
	void GameApplication::OnInit() {
		Core::Application::OnInit();
		LoadTestScene();
	}
	
	void GameApplication::LoadTestScene() {
		auto& scene = GetScene();
		auto& window = GetWindow();

		Core::Shader vert{ "assets/shaders/vertex.glsl", Core::ShaderType::Vertex };
		Core::Shader frag{ "assets/shaders/fragment.glsl", Core::ShaderType::Fragment };

		auto shaderProgram = std::make_shared<Core::ShaderProgram>();
		shaderProgram->Attach(vert);
		shaderProgram->Attach(frag);
		shaderProgram->Link();

		/*Core::Entity floor = scene.CreateEntity();
		floor.GetTransform().SetScale({ 40.0f, 2.0f, 40.0f });
		scene.AttachPhysicsBox(floor, { 20.0f, 1.0f, 20.0f }, true, { 1.0f, 0.0f, 0.0f, 0.0f });
		scene.AttachMesh(floor, std::move(CreateCubeMesh()), shaderProgram);*/

		Core::Entity ramp = scene.CreateEntity();
		ramp.GetTransform().SetScale({ 40.0f, 2.0f, 40.0f });
		scene.AttachRampBox(ramp, { 1.0f, 0.0f, 0.0f, 0.0f }, true);
		scene.AttachMesh(ramp, CreateRampMesh(), shaderProgram);

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				Core::Entity cube1 = scene.CreateEntity();
				cube1.GetTransform().SetPosition({ (float)i, (float)j + 10.0f, -10.0f });
				scene.AttachPhysicsBox(cube1, { 0.5f, 0.5f, 0.5f }, false, { 1.0f, 0.0f, 0.0f, 0.0f });
				scene.AttachMesh(cube1, std::move(CreateCubeMesh()), shaderProgram);
			}
		}

		Core::Entity playerEntity = scene.CreateEntity();
		playerEntity.GetTransform().SetPosition(Core::Vec3{ 0.0f, 10.0f, 5.0f });
		scene.AttachCharacterBox(playerEntity, { 0.5f, 1.0f, 0.5f }, { 1.0f, 0.0f, 0.0f, 0.0f });
		scene.AttachScript<PlayerControllerScript>(playerEntity);

		Core::Entity cameraEntity = scene.CreateEntity();
		auto& camera = scene.CreateCamera(cameraEntity, glm::pi<float>() / 4.0f, window.GetWidth(), window.GetHeight(), 0.1f, 1000.0f);
		scene.AttachScript<CameraControllerScript>(cameraEntity);
		scene.SetActiveCamera(cameraEntity);

		scene.SetParent(playerEntity, cameraEntity);
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

	Core::Mesh GameApplication::CreateRampMesh() {
		// Ramp dimensions: width = 1 (X), depth = 1 (Z), height back = 1 (Y), height front = 0.
		// Back edge at z = -0.5 is high (y = +0.5), front edge at z = +0.5 is low (y = -0.5).
		Core::Vertex vertices[] = {
			// Back edge (high)
			// index: 0
			{ {-0.5f,  10.5f, -0.5f}, {1.0f, 0.0f, 0.0f} }, // back-left-top
			// index: 1
			{ { 0.5f,  10.5f, -0.5f}, {0.0f, 1.0f, 0.0f} }, // back-right-top

			// Front edge (low)
			// index: 2
			{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} }, // front-left-bottom
			// index: 3
			{ { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f} }, // front-right-bottom

			// Bottom back (to form vertical back face)
			// index: 4
			{ {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f} }, // back-left-bottom
			// index: 5
			{ { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f} }, // back-right-bottom
		};

		// Triangles:
		// - top slope (quad split into 2 tris): 0,1,3,2
		// - left side: 0,2,4
		// - right side: 1,5,3
		// - back face (quad 0,1,5,4)
		// - bottom face (quad 4,5,3,2)

		unsigned int indices[] = {
			// top slope
			0, 1, 3,
			3, 2, 0,

			// left side
			0, 2, 4,

			// right side
			1, 5, 3,

			// back face
			0, 5, 1,
			0, 4, 5,

			// bottom face
			4, 3, 5,
			4, 2, 3
		};

		return Core::Mesh{ vertices, sizeof(vertices), indices, sizeof(indices) };
	}
}