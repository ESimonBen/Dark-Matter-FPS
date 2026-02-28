#pragma once
#include <Application.h>

namespace DarkMatter {
	class GameApplication : public Core::Application {
	protected:
		void OnInit() override;
		void LoadTestScene();
		Core::Mesh CreateCubeMesh();
		Core::Mesh CreateRampMesh();
	};
}