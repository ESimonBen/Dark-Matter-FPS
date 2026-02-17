#pragma once
#include <Scene/Scene.h>

namespace Core {
	class RenderSystem {
	public:
		static void OnRender(Scene& scene, Renderer& renderer, float alpha);
	};
}