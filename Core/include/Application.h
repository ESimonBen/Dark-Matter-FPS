#pragma once
#include "Window.h"
#include "Scene/Scene.h"
#include <memory>

namespace Core {
	class Application {
		using EventFunc = std::function<void(Event&)>;

	public:
		Application();
		virtual ~Application();
		void Run();

	protected:
		virtual void OnInit();
		virtual void OnUpdate(float dt);
		virtual void OnEvent(Event& event);
		virtual void OnShutdown();
		void LoadTestScene();
		std::unique_ptr<Mesh> CreateCubeMesh();

	private:
		std::unique_ptr<Window> window;
		std::unique_ptr<Scene> scene;
		float m_LastFrameTime = 0;
		float m_DeltaTime = 0;
		Renderer renderer;
	};
}